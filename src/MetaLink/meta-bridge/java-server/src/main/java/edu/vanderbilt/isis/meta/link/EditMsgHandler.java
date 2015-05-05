package edu.vanderbilt.isis.meta.link;

import edu.vanderbilt.isis.meta.MetaLinkMsg;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;

import io.netty.channel.SimpleChannelInboundHandler;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


public class EditMsgHandler extends SimpleChannelInboundHandler<MetaLinkMsg.Edit> {
    private static final Logger logger = LoggerFactory
            .getLogger(AssemblyDesignBridgeServer.class);

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) {
        logger.warn("exception caught {}", cause.getLocalizedMessage());
        try {
            this.channelInactive(ctx);
        } catch (Exception ex) {
            logger.warn("could not deactivate the channel", ex);
        }
    }

    @Override
    protected void channelRead0(ChannelHandlerContext ctx, MetaLinkMsg.Edit edit) throws Exception {
        EditMsgDistributor.INSTANCE.distribute(ctx, edit);
    }

    /**
     * Register with the distributor.
     */
    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        super.channelActive(ctx);
        logger.info("channel activated {}", ctx);
        EditMsgDistributor.INSTANCE.register(ctx);
    }

    /**
     * Unregister with the distributor.
     */
    @Override
    public void channelInactive(ChannelHandlerContext ctx) throws Exception {
        logger.info("channel deactivated {}", ctx);
        EditMsgDistributor.INSTANCE.unregister(ctx);
        super.channelInactive(ctx);
        ctx.close();
    }

}