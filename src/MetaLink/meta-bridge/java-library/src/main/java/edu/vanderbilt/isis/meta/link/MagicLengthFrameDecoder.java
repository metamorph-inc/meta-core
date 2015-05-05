package edu.vanderbilt.isis.meta.link;

import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.ByteToMessageDecoder;
import io.netty.buffer.ByteBufUtil;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.List;
import java.util.zip.CRC32;

import org.apache.commons.codec.binary.Hex;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * This frame decoder scans for the "magic".
 * <table>
 * <tr><th>size (bytes)</th><th>encoding</th><th>purpose</th></tr>
 * <tr><td>4</td><td>0xdeadbeef</td><td>magic indicates start of frame </td></tr>
 * <tr><td>4</td><td>big endian 32 bit integer, bytes</td><td>size of the payload</td></tr>
 * <tr><td>1</td><td>error code</td><td>error</td></tr>
 * <tr><td>1</td><td>8 bit integer, higher is greater</td><td>priority</td></tr>
 * <tr><td>2</td><td>bits</td><td>reserved</td></tr>
 * <tr><td>4</td><td>crc32 checksum</td><td>payload validation</td></tr>
 * <tr><td>4</td><td>crc32 checksum</td><td>header validation (previous 16 bytes)</td></tr>
 * <tr><td>(size of the payload)</td><td>protocol buffer bytes</td><td>payload</td></tr>
 * <tr><td>4</td><td>crc32 checksum</td><td>payload validation (repeated)</td></tr>
 * </table>
 */
public class MagicLengthFrameDecoder extends ByteToMessageDecoder {
    static final int MAGIC_NUMBER = 0xdeadbeef;
    static final byte[] MAGIC_NUMBER_ARRAY;

    static {
        final ByteBuffer buff = ByteBuffer.allocate(4);
        buff.order(ByteOrder.BIG_ENDIAN).putInt(MAGIC_NUMBER);
        MAGIC_NUMBER_ARRAY = buff.array();
    }

    private static final Logger cookedLogger = LoggerFactory
            .getLogger("meta.link.frame.decoder.cooked");
    private static final Logger rawLogger = LoggerFactory
            .getLogger("meta.link.frame.decoder.raw");


    @Override
    protected void decode(ChannelHandlerContext context, ByteBuf in,  List<Object> out) throws Exception {
        if (in.readableBytes() < 20) {
            cookedLogger.debug("cannot decode, not a full header yet, only {} bytes of {}",
                    in.readableBytes(), in.capacity());
            return;
        }
        cookedLogger.debug("looking for magic in {} bytes", in.readableBytes());
        rawLogger.debug("looking for magic in stream \n{}", ByteBufUtil.hexDump(in));
        while (0 < in.readableBytes()) {
            if (in.getByte(in.readerIndex()) != MAGIC_NUMBER_ARRAY[0]) {
                final byte notMagic = in.readByte();
                cookedLogger.debug("not magic {}", String.format("%02X ", notMagic));
                continue;
            }
            final ByteBuf wip = in.duplicate().order(ByteOrder.BIG_ENDIAN);
            if (wip.readableBytes() < 20) {
                cookedLogger.debug("not even a full header yet, only {}:{} bytes",
                        in.readableBytes(), wip.readableBytes());
                return;
            }
            final int headerStartPos = wip.readerIndex();
            cookedLogger.debug("header start position {}", headerStartPos);

            final int magicNumber = wip.getInt(in.readerIndex());
            if (magicNumber != MAGIC_NUMBER) {
                cookedLogger.error("Magic number mismatch: {} != {} (expected)",
                        Integer.toHexString(magicNumber), Integer.toHexString(MAGIC_NUMBER));
                in.readByte();
                continue;
            }
            wip.readInt(); // 4
            cookedLogger.trace("skip the magic");

            final int size = wip.readInt(); // 8

            @SuppressWarnings("unused")
            final byte priority = wip.readByte(); // 9
            @SuppressWarnings("unused")
            final byte error = wip.readByte(); // 10

            /** two reserved bytes; not used */
            wip.readBytes(2); // 12

            final int payloadChecksum = wip.readInt(); // 16
            cookedLogger.debug("payload checksum {}", Integer.toHexString(payloadChecksum));

            final int headerLength = wip.readerIndex() - headerStartPos;
            cookedLogger.debug("header length {}", headerLength);
            final byte[] header = new byte[headerLength];
            wip.getBytes(headerStartPos, header);
            cookedLogger.debug("raw header {}", ByteBufUtil.hexDump(wip, headerStartPos, headerLength));

            cookedLogger.trace("verify header checksum");
            final CRC32 crc = new CRC32();
            crc.update(header, 0, header.length);
            final int computedHeaderChecksum = (int) crc.getValue();

            final int headerChecksum = wip.readInt(); // 18
            if (headerChecksum != computedHeaderChecksum) {
                cookedLogger.error("Header checksum mismatch: {} (passed) != {} (computed)",
                        Integer.toHexString(headerChecksum), Integer.toHexString(computedHeaderChecksum));
                continue;
            }
            cookedLogger.debug("header checksum {}", Integer.toHexString(headerChecksum));

            if (wip.readableBytes() < size+4) {
                cookedLogger.debug("not enough data to continue: {} < {}", wip.readableBytes(), size);
                return;
            }

            final byte[] payload = new byte[size];
            cookedLogger.debug("position in buffer {}", wip.readerIndex());
            wip.readBytes(payload, 0, size);
            cookedLogger.debug("payload size [{}]", size);
            rawLogger.debug("payload \n{}", Hex.encodeHexString(payload));
            cookedLogger.debug("position in buffer {}", wip.readerIndex());
            final int payloadChecksumTrailer = wip.readInt();
            cookedLogger.debug("payload checksum trailer {}", Integer.toHexString(payloadChecksumTrailer));
            cookedLogger.debug("position in buffer {}", wip.readerIndex());

            final CRC32 dataCrc = new CRC32();
            dataCrc.update(payload);
            int computedPayloadChecksum = (int) dataCrc.getValue();

            if (payloadChecksum != computedPayloadChecksum || payloadChecksum != payloadChecksumTrailer) {
                cookedLogger.error("Payload checksum mismatch: passed[{}] computed[{}] trailer[{}])",
                        Integer.toHexString(payloadChecksum),
                        Integer.toHexString(computedPayloadChecksum),
                        Integer.toHexString(payloadChecksumTrailer));
            }

            in.readerIndex(wip.readerIndex());
            out.add(Unpooled.wrappedBuffer(payload));
        }
    }

}
