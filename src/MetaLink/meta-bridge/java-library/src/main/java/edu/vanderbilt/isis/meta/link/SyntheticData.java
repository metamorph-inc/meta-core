package edu.vanderbilt.isis.meta.link;

import com.google.protobuf.TextFormat;
import edu.vanderbilt.isis.meta.MetaLinkMsg;
import io.netty.channel.ChannelHandlerContext;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.*;
import java.util.concurrent.atomic.AtomicReference;

/**
 * Created with IntelliJ IDEA.
 * User: feisele
 * Date: 7/31/13
 * Time: 1:41 PM
 * To change this template use File | Settings | File Templates.
 */
public class SyntheticData {

    /**
     * These delimiters are used to split up the playback file.
     * Each message type begins with its relevant delimeter.
     * The BEGIN delimiter marks the end of the meta data for the message.
     */
    public enum MessageType {
        EOF(0, "<NO DELIMETER other than end of file>"),
        EDIT(1, "== EDIT DELIMITER : \"9290EECE-9B5A-4310-8F5B-14C248B996A4\" =="),
        COMMENT(2, "== COMMENT DELIMITER : \"9290EECE-9B5A-4310-8F5B-14C248B996A4\" =="),
        META(3, "== BEGIN ==");

        final public String delimiter;
        final public int code;

        private MessageType(int code, String delimiter) {
            this.code = code;
            this.delimiter = delimiter;
        }
    }

    public interface Deliverer {
        public void deliver(final MetaLinkMsg.Edit edit);
        public String originator();
    }



    static public String decodeComment(Logger logger, final String msg) {
        logger.info("comment: \n{}", msg);
        return msg;
    }

    /**
     * <p>
     * Decode the text serialized message.
     * Send it to the provided channel context.
     * </p>
     *
     * @param msgStr
     */
    static public MetaLinkMsg.Edit.Builder decodeEdit(Logger logger, final String msgStr) {
        logger.trace("decoding edit");
        final MetaLinkMsg.Edit.Builder builder;
        try {
            builder =  MetaLinkMsg.Edit.newBuilder();
            TextFormat.merge(msgStr, builder);
        } catch (TextFormat.ParseException ex) {
            logger.warn("unparsable protobuf message \n{}", msgStr, ex);
            return null;
        }
        return builder;
    }

    /**
     *  <p>
     *     Write the message to the recorder file for later study and playback.
     *     This involves converting the binary payload to a textual representation.
     * </p>
     * <p>
     *     The edit message is passed so that topic filtering can be performed.
     * </p>
     *
     * @param playbackFile
     */
    public static boolean playback(final Logger logger, final File playbackFile, final Deliverer deliverer) {
        logger.info("playback ");
        if (playbackFile == null) {
            return false;
        }
        final BufferedReader reader;
        try {
            final FileReader fstream = new FileReader(playbackFile);
            reader = new BufferedReader(fstream);
        } catch (FileNotFoundException ex) {
            logger.error("could not read playback file {}", playbackFile, ex);
            return false;
        }

        /**
         * a simple state machine.
         */
        StringBuilder msg = new StringBuilder();
        MessageType mode = MessageType.COMMENT;
        while (! MessageType.EOF.equals(mode)) {
            try {
                final String line = reader.readLine();
                final MessageType nMode;
                if (line == null) {
                    nMode = MessageType.EOF;
                }  else if (MessageType.COMMENT.delimiter.equals(line)) {
                    nMode = MessageType.COMMENT;
                }  else if (MessageType.EDIT.delimiter.equals(line)) {
                    nMode = MessageType.EDIT;
                }  else if (msg == null) {
                    continue;
                } else if (MessageType.META.delimiter.equals(line)) {
                    logger.info("metadata {}", msg.toString());
                    // TODO something with the meta data
                    msg = new StringBuilder();
                    continue;
                }  else {
                    msg.append(line).append('\n');
                    continue;
                }
                logger.debug("play: {} {}", mode, msg.substring(0,(25 < msg.length() ? 25 : msg.length())));
                switch (mode) {
                    case COMMENT:
                        final String comment = SyntheticData.decodeComment(logger, msg.toString());
                        break;
                    case EDIT: {
                        final MetaLinkMsg.Edit.Builder editBuilder = SyntheticData.decodeEdit(logger, msg.toString());
                        editBuilder.addOrigin(deliverer.originator());
                        deliverer.deliver(editBuilder.build());
                        break;
                    }
                    default:
                        logger.warn("unknown mode {}", mode);
                        continue;
                }
                mode = nMode;
                msg = new StringBuilder();

            } catch (IOException ex) {
                logger.error("could not process line from playback file {}", playbackFile, ex);
                return false;
            }
        }
        return true;
    }


    public static MetaLinkMsg.Edit parseString(final Logger logger, final String message) {
        final MetaLinkMsg.Edit.Builder builder = MetaLinkMsg.Edit.newBuilder();
        builder.setEditMode(MetaLinkMsg.Edit.EditMode.POST);
        try {
            TextFormat.merge(message, builder);
        } catch (TextFormat.ParseException ex) {
            logger.error("bad protobuf text format message {}", message, ex);
        }
        return builder.build();
    }

    public static class Recorder {
        final static Logger logger = LoggerFactory.getLogger("synthetic data logger") ;
        private AtomicReference<BufferedWriter> recordStream = new AtomicReference<BufferedWriter>();

        public Recorder(final File recordFile) {
            if (recordFile == null)  {
                logger.info("no recording file specified");
                return;
            }
            synchronized (this.recordStream) {
                try {
                    final File directory = recordFile.getParentFile();
                    if (directory == null) {
                        logger.warn("directory is null");
                    } else if (! directory.exists()) {
                        logger.warn("recorder directory does not exist: {}", directory);
                        if (! directory.mkdirs()) {
                            logger.warn("recorder directory could not be made: {}", directory);
                            return;
                        }
                    }
                    final FileWriter fstream = new FileWriter(recordFile, true);
                    if (fstream != null) {
                        logger.info("output file opened: {}", recordFile);
                    }
                    final BufferedWriter andSet = this.recordStream.getAndSet(new BufferedWriter(fstream));
                    if (andSet != null) {
                        andSet.close();
                    }

                } catch (IOException ex) {
                    logger.warn("the recording file path provided is not usable: {}", recordFile, ex);
                }
            }
        }
        /**
         *  <p>
         *     Write the message to the recorder file for later study and playback.
         *     This involves converting the binary payload to a textual representation.
         * </p>
         *
         * @param sourceCtx
         * @param edit
         */
        public void record(ChannelHandlerContext sourceCtx, final MetaLinkMsg.Edit edit) {
            synchronized (this.recordStream) {
                final BufferedWriter writer = this.recordStream.get();
                if (writer == null) {
                    logger.trace("no recorder");
                    return;
                }
                final String output = TextFormat.printToString(edit);
                try {
                    writer.write(MessageType.EDIT.delimiter);
                    writer.write('\n');

                    writer.write(writeMetaData());
                    writer.write('\n');
                    writer.write(MessageType.META.delimiter);
                    writer.write('\n');

                    writer.write(output);
                    writer.write('\n');
                    writer.flush();

                } catch (IOException ex) {
                    logger.error("could not write to recorder file");
                }   finally {
                    logger.info("edit recorded: \n {}", output);
                }
            }
        }


        /**
         * Write some metadata about the message.
         * The
         * @return
         */
        private String writeMetaData() {
            final StringBuilder builder = new StringBuilder();
            builder.append("timestamp: ").append(System.currentTimeMillis());
            return builder.toString();
        }

        public void stopRecording() {
            synchronized (this.recordStream) {
                final BufferedWriter writer = this.recordStream.getAndSet(null);
                if (writer == null) return;
                try {
                    writer.close();
                } catch (IOException ex) {
                    logger.error("could not close record file", ex);
                }
            }
        }
    }


}
