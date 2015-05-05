package edu.vanderbilt.isis.meta.link;

import io.netty.buffer.ByteBuf;
import io.netty.buffer.ByteBufUtil;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.MessageToByteEncoder;

import java.nio.ByteOrder;
import java.util.zip.CRC32;

import org.apache.commons.codec.binary.Hex;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 *   The frame is defined in MagicLengthFrameDecoder.
 *
 */
public class MagicLengthFrameEncoder extends MessageToByteEncoder<ByteBuf> {
    private static final Logger cookedLogger = LoggerFactory
            .getLogger("meta.link.frame.encoder.cooked");
    private static final Logger rawLogger = LoggerFactory
            .getLogger("meta.link.frame.encoder.raw");

    @Override
    protected void encode(ChannelHandlerContext ctx, ByteBuf msg, ByteBuf out) throws Exception {

        try {

            final ByteBuf headerBuf = Unpooled.buffer(20).order(ByteOrder.BIG_ENDIAN);
            headerBuf.writeInt(MagicLengthFrameDecoder.MAGIC_NUMBER);
            final int payloadSize = msg.readableBytes();
            final byte[] payload = new byte[payloadSize];
            msg.readBytes(payload);
            headerBuf.writeInt(payloadSize);

            //Message priority
            headerBuf.writeByte(0);
            //Error code
            headerBuf.writeByte(0);
            //Reserved byte
            headerBuf.writeByte(0);
            //Reserved byte
            headerBuf.writeByte(0);

            // payload CRC32
            final CRC32 payloadCrc = new CRC32();
            payloadCrc.update(payload);
            final int payloadChecksum = (int) payloadCrc.getValue();
            headerBuf.writeInt(payloadChecksum);

            // Header CRC32 (all 16 bytes of the header, not including itself)
            final CRC32 headerCrc = new CRC32();
                final byte[] headerArray = headerBuf.array();
            headerCrc.update(headerArray, 0, 16);
            final int headerChecksum = (int) headerCrc.getValue();
            headerBuf.writeInt(headerChecksum);

            out.writeBytes(headerBuf);
            cookedLogger.info("header: size {}, payload-check {}, header-check {}",
                    payloadSize, Integer.toHexString(payloadChecksum),
                    Integer.toHexString(headerChecksum));
            cookedLogger.info("wrote the header: {}", ByteBufUtil.hexDump(headerBuf.resetReaderIndex()));

            out.writeBytes(payload);
            cookedLogger.info("wrote the payload size: {}", payloadSize);
            rawLogger.trace("wrote payload {}", Hex.encodeHexString(payload));

            final ByteBuf trailerBuf = Unpooled.buffer(4).order(ByteOrder.BIG_ENDIAN);
            trailerBuf.writeInt(payloadChecksum);
            out.writeBytes(trailerBuf);
            cookedLogger.info("wrote the trailer checksum {}", ByteBufUtil.hexDump(trailerBuf.resetReaderIndex()));


        } catch (Exception ex) {
            cookedLogger.error("exception", ex);
        }
    }
}