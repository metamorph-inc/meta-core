package edu.vanderbilt.isis.meta.link;

import com.google.protobuf.ByteString;
import edu.vanderbilt.isis.meta.MetaLinkMsg;
import io.netty.channel.ChannelHandlerContext;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.File;
import java.net.SocketAddress;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

public enum EditMsgDistributor {
    INSTANCE;
    private static final Logger majorLogger = LoggerFactory
            .getLogger("meta.link.msg.distributor.major");
    private static final Logger detailLogger = LoggerFactory
            .getLogger("meta.link.msg.distributor.detail");

    final static ByteString NO_INTEREST_PRESENT = ByteString.copyFromUtf8("B000001");
    final static String METALINK_BRIDGE_NAME = "metalink-bridge";

    private final AtomicReference<SyntheticData.Recorder> recorder;
    private final AtomicReference<File> playFile;

    private final AtomicInteger sequenceNumber;
    /**
     * The following two maps
     */
    private final Map<SocketAddress, Interest.ChannelInterestSet> channelInterestMap;
    private final Map<Interest, Interest.InterestedChannelSet> interestedChannelMap;

    private EditMsgDistributor() {
        this.channelInterestMap = new HashMap<SocketAddress, Interest.ChannelInterestSet>();
        this.interestedChannelMap = new HashMap<Interest, Interest.InterestedChannelSet>();

        this.recorder = new AtomicReference<SyntheticData.Recorder>() ;
        this.playFile = new AtomicReference<File>();

        this.sequenceNumber = new AtomicInteger(0);
    }

    /**
     * Send the data to all channels except the incoming channel.
     * <p>
     *     Write the message to the recorder file for later study and playback.
     *     This involves converting the binary payload to a textual representation.
     * </p>
     * <p>
     *     If the inbound message is an INTEREST message then it is not sent.
     *     Instead it is responded to in a way indicated by the ?.
     * </p>
     *
     * @param sourceCtx
     * @param originalMsg
     */
    public void distribute(ChannelHandlerContext sourceCtx, final MetaLinkMsg.Edit originalMsg) {
        majorLogger.trace("incoming message to distribute(): {}\n {}", sourceCtx.channel().remoteAddress(), originalMsg);
        if (originalMsg == null) {
            majorLogger.warn("attempt to distribute null message");
            return;
        }
        try {
            final SyntheticData.Recorder recorder = this.recorder.get();

            final MetaLinkMsg.Edit.Builder editBuilder = originalMsg.toBuilder();
            final MetaLinkMsg.Edit msg;
            if (originalMsg.getModeCount() < 1) {
                if (originalMsg.hasEditMode()) {
                    editBuilder.addMode(originalMsg.getEditMode());
                    msg = editBuilder.build();
                } else {
                    majorLogger.warn("no edit mode provided, assuming POST");
                    editBuilder.addMode(MetaLinkMsg.Edit.EditMode.POST);
                    editBuilder.setEditMode(MetaLinkMsg.Edit.EditMode.POST);
                    msg = editBuilder.build();
                }
            }  else  {
                if (originalMsg.hasEditMode()) {
                    if (originalMsg.getEditMode() == originalMsg.getMode(originalMsg.getModeCount()-1)) {
                        msg = editBuilder.build();
                    } else {
                        majorLogger.info("edit mode mismatch, set the last mode");
                        editBuilder.addMode(originalMsg.getEditMode());
                        msg = editBuilder.build();
                    }
                } else {
                    majorLogger.info("edit mode provided, using the last mode");
                    editBuilder.setEditMode(originalMsg.getMode(originalMsg.getModeCount()-1));
                    msg = editBuilder.build();
                }
            }

            if (null != recorder) {
                recorder.record(sourceCtx, msg);
            }
            final String trackingGuid = msg.hasGuid() ? msg.getGuid().toString() : UUID.randomUUID().toString();
            final String origin = (msg.getOriginCount() > 0) ? msg.getOrigin(0) : "";

            switch (msg.getEditMode()) {
                case NOTICE:
                {
                    final Interest topic = new Interest(msg.getTopicList());
                    final Interest.InterestedChannelSet channelSet = this.interestedChannelMap.get(topic);
                    if (channelSet == null) {
                        majorLogger.warn("no interested channel set");
                        editBuilder
                                .addMode(MetaLinkMsg.Edit.EditMode.NOTICE)
                                .setEditMode(MetaLinkMsg.Edit.EditMode.NOTICE)
                                .addOrigin(METALINK_BRIDGE_NAME)
                                .setSequence(sequenceNumber.incrementAndGet()) ;

                        final MetaLinkMsg.Notice.Builder noticeBuilder = MetaLinkMsg.Notice.newBuilder()
                                .setNoticeMode(MetaLinkMsg.Notice.NoticeMode.ACK)
                                .setCode(NO_INTEREST_PRESENT)
                                .setMsg("no interest in topic")
                                .setId(msg.getGuid());
                        editBuilder.addNotices(noticeBuilder);

                        final MetaLinkMsg.Edit updatedEdit = editBuilder.build();
                        majorLogger.trace("notice no-interest notice {} to {}",
                                trackingGuid, sourceCtx.channel().remoteAddress());
                        sourceCtx.write(updatedEdit);
                        sourceCtx.flush();
                        detailLogger.info("update edit\n{}", updatedEdit);

                        if (null != recorder) {
                            recorder.record(sourceCtx, updatedEdit);
                        }
                        break;
                    }
                    majorLogger.trace("posting notice to all interested channels except source");
                    boolean anyInterest = false;
                    for (final ChannelHandlerContext ctx : channelSet.getContextSet()) {
                        if (isSameChannel(ctx, sourceCtx)) {
                            continue;
                        }
                        anyInterest = true;
                        majorLogger.trace("notice forward {} to {}",
                                trackingGuid, ctx.channel().remoteAddress());
                        ctx.write(msg);
                        ctx.flush();
                        detailLogger.info("message sent:\n{}", msg);
                    }

                    if (!anyInterest) {
                        editBuilder
                                .addMode(MetaLinkMsg.Edit.EditMode.NOTICE)
                                .setEditMode(MetaLinkMsg.Edit.EditMode.NOTICE)
                                .addOrigin(METALINK_BRIDGE_NAME)
                                .setSequence(sequenceNumber.incrementAndGet()) ;

                        final MetaLinkMsg.Notice.Builder noticeBuilder = MetaLinkMsg.Notice.newBuilder()
                                .setNoticeMode(MetaLinkMsg.Notice.NoticeMode.ACK)
                                .setCode(NO_INTEREST_PRESENT)
                                .setMsg("no pending interest for topic")
                                .setId(msg.getGuid());
                        editBuilder.addNotices(noticeBuilder);

                        final MetaLinkMsg.Edit updatedEdit = editBuilder.build();
                        majorLogger.trace("notice no interest {} to {}",
                                trackingGuid, sourceCtx.channel().remoteAddress());
                        sourceCtx.write(updatedEdit);
                        sourceCtx.flush();
                        detailLogger.info("message sent:\n{}", updatedEdit);

                        majorLogger.trace("cannot post, no other interest \n{}", updatedEdit);
                        if (null != recorder) {
                            recorder.record(sourceCtx, updatedEdit);
                        }
                    }

                    break;
                }
                case POST:
                {
                    final Interest topic = new Interest(msg.getTopicList());
                    final Interest.InterestedChannelSet channelSet = this.interestedChannelMap.get(topic);
                    if (channelSet == null || channelSet.size()==0) {
                        majorLogger.warn("no interested channel set");
                        editBuilder
                                .addMode(MetaLinkMsg.Edit.EditMode.NOTICE)
                                .setEditMode(MetaLinkMsg.Edit.EditMode.NOTICE)
                                .addOrigin(METALINK_BRIDGE_NAME)
                                .setSequence(sequenceNumber.incrementAndGet()) ;

                        final MetaLinkMsg.Notice.Builder noticeBuilder = MetaLinkMsg.Notice.newBuilder()
                                .setNoticeMode(MetaLinkMsg.Notice.NoticeMode.ACK)
                                .setCode(NO_INTEREST_PRESENT)
                                .setMsg("no interest in the posted topic")
                                .setId(msg.getGuid());
                        editBuilder.addNotices(noticeBuilder);

                        final MetaLinkMsg.Edit updatedEdit = editBuilder.build();
                        majorLogger.trace("post no-interest {} to {}",
                                trackingGuid, sourceCtx.channel().remoteAddress());
                        sourceCtx.write(updatedEdit);
                        sourceCtx.flush();
                        detailLogger.info("message sent:\n{}", updatedEdit);

                        if (null != recorder) {
                            recorder.record(sourceCtx, updatedEdit);
                        }
                        break;
                    } else {
                        majorLogger.trace("posting edit to all interested channels except source");
                        for (final ChannelHandlerContext ctx : channelSet.getContextSet()) {
                            if (isSameChannel(ctx, sourceCtx)) {
                                continue;
                            }
                            majorLogger.trace("post all {} to {}",
                                    trackingGuid, ctx.channel().remoteAddress());
                            ctx.write(msg);
                            ctx.flush();
                            detailLogger.info("message sent:\n{}", msg);
                        }
                    }
/*
                    if (! anyInterest) {
                        editBuilder
                                .addMode(MetaLinkMsg.Edit.EditMode.NOTICE)
                                .setEditMode(MetaLinkMsg.Edit.EditMode.NOTICE)
                                .addOrigin(METALINK_BRIDGE_NAME)
                                .setSequence(sequenceNumber.incrementAndGet());

                        final MetaLinkMsg.Notice.Builder noticeBuilder = MetaLinkMsg.Notice.newBuilder()
                                .setNoticeMode(MetaLinkMsg.Notice.NoticeMode.ACK)
                                .setCode(NO_INTEREST_PRESENT)
                                .setMsg("no pending interest for topic")
                                .setId(msg.getGuid());
                        editBuilder.addNotices(noticeBuilder);

                        final MetaLinkMsg.Edit updatedEdit = editBuilder.build();
                        majorLogger.trace("post no interest {} to {}",
                                trackingGuid, sourceCtx.channel().remoteAddress());
                        sourceCtx.write(updatedEdit);
                        sourceCtx.flush();
                        detailLogger.info("message sent:\n{}", updatedEdit);

                        majorLogger.trace("cannot post, no other interest \n{}", updatedEdit);
                        if (null != recorder) {
                            recorder.record(sourceCtx, updatedEdit);
                        }
                    }
*/
                    break;
                }
                case INTEREST:
                {
                    final Interest interest = new Interest(msg.getTopicList());
                    playback(sourceCtx, interest);

                    majorLogger.trace("determine if there is a client which already is interested?");
                    final Interest.InterestedChannelSet ics = this.interestedChannelMap.get(interest);
                    boolean anyInterest = false;
                    if (ics != null) {
                        for (final ChannelHandlerContext ctx : ics.getContextSet()) {
                            if (isSameChannel(ctx, sourceCtx)) {
                                continue;
                            }
                            anyInterest = true;
                            majorLogger.trace("interest in {} from {} to {}", trackingGuid,
                                    ctx.channel().remoteAddress(),
                                    sourceCtx.channel().remoteAddress());
                            sourceCtx.write(msg);
                            sourceCtx.flush();
                            detailLogger.info("message sent:\n{}", msg);
                        }
                    }
                    if (! anyInterest) {
                        editBuilder
                                .addMode(MetaLinkMsg.Edit.EditMode.NOTICE)
                                .setEditMode(MetaLinkMsg.Edit.EditMode.NOTICE)
                                .addOrigin(METALINK_BRIDGE_NAME)
                                .setSequence(sequenceNumber.incrementAndGet());

                        final MetaLinkMsg.Notice.Builder noticeBuilder = MetaLinkMsg.Notice.newBuilder()
                                .setNoticeMode(MetaLinkMsg.Notice.NoticeMode.ACK)
                                .setCode(NO_INTEREST_PRESENT)
                                .setMsg("no pending interest for topic")
                                .setId(msg.getGuid());
                        editBuilder.addNotices(noticeBuilder);

                        final MetaLinkMsg.Edit updatedEdit = editBuilder.build();
                        majorLogger.trace("interest notice {} to {}",
                                trackingGuid, sourceCtx.channel().remoteAddress());
                        sourceCtx.write(updatedEdit);
                        sourceCtx.flush();
                        detailLogger.info("message sent:\n{}", updatedEdit);

                        majorLogger.trace("no other interest");
                        if (null != recorder) {
                           recorder.record(sourceCtx, updatedEdit);
                        }
                    }
                    setInterest(sourceCtx, interest);
                    majorLogger.trace("interest recorded");

                    majorLogger.trace("expressing interest to all channels except source");
                    for (final Map.Entry<SocketAddress, Interest.ChannelInterestSet> entry : this.channelInterestMap.entrySet()) {
                        final ChannelHandlerContext ctx = entry.getValue().context;
                        if (isSameChannel(ctx, sourceCtx)) {
                            continue;
                        }
                        majorLogger.trace("interest {} to {}",
                                trackingGuid, ctx.channel().remoteAddress());
                        ctx.write(msg);
                        ctx.flush();
                        detailLogger.info("message sent:\n{}", msg);
                    }
                    break;
                }
                case DISINTEREST:
                {
                    final Interest interest = new Interest(msg.getTopicList());

                    majorLogger.trace("expressing disinterest to all channels except source");
                    for (final Map.Entry<SocketAddress, Interest.ChannelInterestSet> entry : this.channelInterestMap.entrySet()) {
                        final ChannelHandlerContext ctx = entry.getValue().context;
                        if (isSameChannel(ctx, sourceCtx)) {
                            continue;
                        }
                        majorLogger.trace("disinterest {} to {}", trackingGuid, ctx.channel().remoteAddress());
                        ctx.write(msg);
                        ctx.flush();
                        detailLogger.info("message sent:\n{}", msg);
                    }
                    setDisinterest(sourceCtx, interest);
                }
            }
        } catch (Exception ex) {
            majorLogger.warn("problem distributing message", ex);
        }
        majorLogger.info("MESSAGE PROCESSED");
    }

    static private boolean isSameChannel(final ChannelHandlerContext lhs, final ChannelHandlerContext rhs)  {
        return (lhs.channel().remoteAddress().equals(rhs.channel().remoteAddress()));
    }

    /**
     *  <p>
     *     Reads the messages from the playback file.
     *     This involves converting the textual representation into a binary payload.
     * </p>
     * <p>
     *     The edit message is passed so that topic filtering can be performed.
     * </p>
     *
     * @param ctx
     * @param interest
     */
    private void playback(final ChannelHandlerContext ctx, final Interest interest) {
        majorLogger.info("playback ");
        final SyntheticData.Deliverer deliverer = new SyntheticData.Deliverer() {
            final ChannelHandlerContext ctx_ = ctx;
            final String originator_ = ctx.channel().localAddress().toString();
            @Override
            public void deliver(final MetaLinkMsg.Edit edit) {
                if (0 != interest.compareTopic(edit.getTopicList())) {
                    return;
                }
                final String trackingGuid = edit.hasGuid() ? edit.getGuid().toString() : UUID.randomUUID().toString();
                majorLogger.trace("playback {} to {}", trackingGuid, ctx_.channel().remoteAddress());
                ctx_.write(edit);
                ctx_.flush();
                detailLogger.info("playback message sent:\n{}", edit);
            }
            @Override
            public String originator() {
                 return originator_;
            }
        };

        SyntheticData.playback(majorLogger, this.playFile.get(), deliverer);
    }


    /**
     * As new connections/channels are made they are registered.
     * Once registered all interest messages (without owner) will
     * be sent to the channel.
     *
     * @param ctx
     */
    public void register(final ChannelHandlerContext ctx) {
        final SocketAddress key = ctx.channel().remoteAddress();
        majorLogger.info("registration {} expression of interest pending ", key);
        final Interest.ChannelInterestSet interestSet =  new Interest.ChannelInterestSet(ctx);
        this.channelInterestMap.put(key, interestSet);
    }

    /**
     * Remove all references to the channel.
     * 1) the map of a channel (socket) to its interests.
     * 2) the map of interests to its channels.
     *
     * As each interest is removed inform all channels
     * having a similar interest.
     *
     * @param sourceCtx
     */
    public void unregister(final ChannelHandlerContext sourceCtx) {
        final SocketAddress key = sourceCtx.channel().remoteAddress();
        majorLogger.info("unregister channel {}", key);
        final Interest.ChannelInterestSet channelInterestSet = this.channelInterestMap.remove(key);
        if (channelInterestSet == null) {
            majorLogger.warn("nothing to unregister in interest map: {}",
                    this.channelInterestMap.keySet() );
            return;
        }
        final String trackingGuid = UUID.randomUUID().toString();
        majorLogger.warn("no interested channel set");

        for ( final Interest interest : channelInterestSet.getInterestList()) {
            if (! this.interestedChannelMap.containsKey(interest)) {
                continue;
            }

            final Interest.InterestedChannelSet interestedChannelSet = this.interestedChannelMap.get(interest);
            interestedChannelSet.removeItem(sourceCtx, interest);
            majorLogger.info("unregistered interest {}", interest);
            // this.interestedChannelMap.put(interest, interestedChannelSet);

            // an implicit DISINTEREST message will be sent to all interested channels

            final MetaLinkMsg.Edit.Builder editBuilder = MetaLinkMsg.Edit.newBuilder()
                    .addMode(MetaLinkMsg.Edit.EditMode.DISINTEREST)
                    .setEditMode(MetaLinkMsg.Edit.EditMode.DISINTEREST)
                    .setGuid(trackingGuid)
                    .setSequence(sequenceNumber.incrementAndGet())
                    .addAllTopic(interest.getTopic())
                    .addOrigin(sourceCtx.channel().remoteAddress().toString())
                    .addOrigin(METALINK_BRIDGE_NAME);
            final MetaLinkMsg.Edit disinterestMsg = editBuilder.build();

            majorLogger.trace("expressing disinterest to all channels except source");
            for (final Map.Entry<SocketAddress, Interest.ChannelInterestSet> entry : this.channelInterestMap.entrySet()) {
                final ChannelHandlerContext ctx = entry.getValue().context;
                if (isSameChannel(ctx, sourceCtx)) {
                    continue;
                }
                majorLogger.trace("disinterest {} to {}", trackingGuid, ctx.channel().remoteAddress());
                ctx.write(disinterestMsg);
                ctx.flush();
                detailLogger.info("message sent:\n{}", disinterestMsg);
            }
        }
    }

    /**
     * Only registered channels may have interest set.
     * Once interest has been set all post messages with matching topic
     * and will be sent to the channel.
     *
     * @param ctx   the channel
     * @param interest  the topic
     */
    public void setInterest(final ChannelHandlerContext ctx, final Interest interest) {
        majorLogger.info("set expression of interest [{}]", interest);

        final SocketAddress key = ctx.channel().remoteAddress();
        if ( ! this.channelInterestMap.containsKey(key)) {
            majorLogger.error("can not express interest from an unregistered client") ;
            // write a notice, to whom?
            return;
        }
        final Interest.ChannelInterestSet interestSet = this.channelInterestMap.get(key);
        interestSet.addItem(ctx, interest);

        final Interest.InterestedChannelSet channelSet;
        if ( (!this.interestedChannelMap.containsKey(interest))
        || (this.interestedChannelMap.get(interest) == null)) {
            channelSet = new Interest.InterestedChannelSet(interest);
            this.interestedChannelMap.put(interest, channelSet);
        } else {
            channelSet = this.interestedChannelMap.get(interest);
        }
        channelSet.addItem(ctx, interest);
    }


    /**
     * Drop the interest for the specified socket.
     *
     * @param ctx
     * @param interest
     */
    public void setDisinterest(final ChannelHandlerContext ctx,  final Interest interest) {
        majorLogger.info("set {} expression of disinterest {}:{}", ctx, interest);

        final SocketAddress key = ctx.channel().remoteAddress();
        if (this.channelInterestMap.containsKey(key)) {
            final Interest.ChannelInterestSet interestSet = this.channelInterestMap.get(key);
            interestSet.removeItem(ctx, interest);
            // this.channelInterestMap.put(key, interestSet);
        }
        if (this.interestedChannelMap.containsKey(interest)) {
            final Interest.InterestedChannelSet channelSet = this.interestedChannelMap.get(interest);
            channelSet.removeItem(ctx, interest);
            // this.interestedChannelMap.put(interest, channelSet);
        }
    }


    /**
     * A non-null file name indicates that playback be enabled.
     * The file must also be present and have the appropriate permissions.
     *
     * @param file
     */
    public void play(final File file) {
        if (file == null)  {
            majorLogger.info("no player file specified");
            return;
        }
        if (! file.exists())  {
            majorLogger.warn("the player file path provided does not exist: {}", file);
            return;
        }
        if (! file.canRead())  {
            majorLogger.warn("the player file path provided is not readable: {}", file);
            return;
        }
        this.playFile.getAndSet(file);
    }

    /**
     * A non-null file name indicates that recording be enabled.
     * The file must also be present and have the appropriate permissions.
     *
     * @param file
     */
    public void record(final File file) {
        if (file == null)  {
            majorLogger.info("no recorder file specified");
            return;
        }
        if (file.exists())  {
            majorLogger.warn("the recorder file is being overwritten", file);
            if (! file.canWrite())  {
                majorLogger.warn("the recorder file path provided is not writable: {}", file);
                return;
            }
        }
        final SyntheticData.Recorder old = this.recorder.getAndSet(new SyntheticData.Recorder(file));
        if (old != null) {
            old.stopRecording();
        }
    }

}
