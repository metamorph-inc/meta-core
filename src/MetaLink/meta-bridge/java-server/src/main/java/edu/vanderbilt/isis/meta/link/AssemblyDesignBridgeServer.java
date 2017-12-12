package edu.vanderbilt.isis.meta.link;

import edu.vanderbilt.isis.meta.MetaLinkMsg;
import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.*;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioServerSocketChannel;
import io.netty.handler.codec.protobuf.ProtobufDecoder;
import io.netty.handler.codec.protobuf.ProtobufEncoder;
import org.apache.commons.cli.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import ch.qos.logback.classic.LoggerContext;
import ch.qos.logback.core.util.StatusPrinter;
import sun.misc.IOUtils;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.net.InetSocketAddress;
import java.net.InetAddress;

public class AssemblyDesignBridgeServer {
    private static final Logger logger = LoggerFactory
            .getLogger("meta.link.bridge.server");
    private int port = 15150;
    private File recordFile = null;
    private File playFile = null;

    public AssemblyDesignBridgeServer() {}

    /**
     * Fire up the server.
     *
     * @throws Exception
     */
    public void start() throws Exception {
        EditMsgDistributor.INSTANCE.record(this.recordFile);
        EditMsgDistributor.INSTANCE.play(this.playFile);

        final EventLoopGroup bossGroup = new NioEventLoopGroup();
        final EventLoopGroup workerGroup = new NioEventLoopGroup();

        try {
            final ServerBootstrap boot = new ServerBootstrap();
            final ChannelInitializer<SocketChannel> initializer = new ChannelInitializer<SocketChannel>() {
                @Override
                public void initChannel(SocketChannel channel) throws Exception {
                    logger.trace("initialize socket");
                    final ChannelPipeline pipeline = channel.pipeline();
                    pipeline.addLast("frameDecoder", new MagicLengthFrameDecoder());
                    pipeline.addLast("frameEncoder", new MagicLengthFrameEncoder());

                    pipeline.addLast("protobufDecoder",
                            new ProtobufDecoder(MetaLinkMsg.Edit.getDefaultInstance()));
                    pipeline.addLast("protobufEncoder", new ProtobufEncoder());

                    pipeline.addLast("distributor", new EditMsgHandler());
                }
            };

            boot.group(bossGroup, workerGroup)
                    .channel(NioServerSocketChannel.class)
                    .option(ChannelOption.SO_BACKLOG, 100)
                    .localAddress(InetAddress.getByName(null), this.port)
                    .option(ChannelOption.TCP_NODELAY, true)
                    .option(ChannelOption.SO_KEEPALIVE, true)
                    .childHandler(initializer);

            final ChannelFuture future = boot.bind().sync();
            logger.info("{} started and listening on {}",
                    AssemblyDesignBridgeServer.class.getCanonicalName(),
                    future.channel().localAddress());
            future.channel().closeFuture().sync();
        } finally {
            bossGroup.shutdownGracefully();
            workerGroup.shutdownGracefully();

            bossGroup.terminationFuture().sync();
            workerGroup.terminationFuture().sync();
        }
    }

    /**
     * Describe how to call the program.
     *
     * @param options
     */
    private static void usage(final Options options) {
        HelpFormatter formatter = new HelpFormatter();
        formatter.printHelp( AssemblyDesignBridgeServer.class.getSimpleName(), options );
        System.out.println("to force the loading of a specific logback.xml file:");
        System.out.println("java -Dlogback.configurationFile=/path/to/config.xml -jar ...");
    }

    /**
     * The entry point.
     *
     * @param args
     * @throws Exception
     */
    public static void main(String[] args) throws Exception {

        final CommandLineParser parser = new BasicParser();
        final Options options = new Options();

        final Option showHelp = OptionBuilder
                .withLongOpt("show-help")
                .withDescription("show the usage information")
                .hasArg(false)
                .create('h');
        options.addOption(showHelp);

        final Option showLogConfig = OptionBuilder
                .withLongOpt("show-log-config")
                .hasArg(false)
                .withDescription("show the logger configuration information")
                .create('L');
        options.addOption(showLogConfig);

        final Option saveLogConfig = OptionBuilder
                .withLongOpt("save-log-config")
                .withDescription("the path to the logback configuration file")
                .hasArg(true)
                .withArgName("LOG-CONFIG-SAVE")
                .withType(File.class)
                .create('l');
        options.addOption(saveLogConfig);


        final Option setPort = OptionBuilder
                .withLongOpt("set-port")
                .withDescription("the port on the target host")
                .hasArg(true)
                .withArgName("PORT")
                .withType(Number.class)
                .create('P');
        options.addOption(setPort);

        final Option startPlay = OptionBuilder
                .withLongOpt("set-playback")
                .withDescription("a file containing a list of messages to be sent to each device as it connects and expresses interest")
                .hasArg(true)
                .withArgName("PLAY")
                .withType(File.class)
                .create('p');
        options.addOption(startPlay);

        final Option startRecord = OptionBuilder
                .withLongOpt("start-record")
                .withDescription("a file into which received messages are stored")
                .hasArg(true)
                .withArgName("RECORD")
                .withType(File.class)
                .create('r');
        options.addOption(startRecord);

        try {
            final CommandLine line = parser.parse(options, args);

            if (line.hasOption(saveLogConfig.getOpt())) {
                InputStream inputStream = null;
                FileOutputStream outputStream = null;

                try {
                    final File logconfigFile = (File)line.getParsedOptionValue(saveLogConfig.getOpt());
                    logger.info("when default logback.xml file is saved no other processing is performed");
                    inputStream = ClassLoader.getSystemResourceAsStream("logback.xml");
                    outputStream = new FileOutputStream(logconfigFile);

                    int read = 0;
                    byte[] bytes = new byte[1024];

                    while ((read = inputStream.read(bytes)) != -1) {
                        outputStream.write(bytes, 0, read);
                    }
                } catch (Exception ex) {
                    logger.error("could not write logback configuration file");
                    return;
                } finally {
                    if (inputStream != null) { inputStream.close();  }
                    if (outputStream != null) { outputStream.close(); }
                }
                return;
            }

            if (line.hasOption(showLogConfig.getOpt())) {
                final LoggerContext lc = (LoggerContext) LoggerFactory.getILoggerFactory();
                StatusPrinter.print(lc);
            }

            if (line.hasOption(showHelp.getOpt())) {
                usage(options);
                return;
            }

            final AssemblyDesignBridgeServer server = new AssemblyDesignBridgeServer();
            server.port = 15150;
            if (line.hasOption(setPort.getOpt())) {
                try {
                    Number portCandidate = (Number)line.getParsedOptionValue(setPort.getOpt());
                    if (portCandidate.longValue() > Integer.MAX_VALUE)  {
                        logger.warn("port value is too large {}", portCandidate);
                    }  else if (portCandidate.longValue() < 0) {
                        logger.warn("port value is too small {}", portCandidate);
                    }  else {
                        server.port = portCandidate.intValue();
                    }
                    logger.info("setting port: {}", server.port);
                } catch (ParseException ex) {
                    logger.error("could not parse port number : {}", line.getOptionValue(setPort.getLongOpt())) ;
                }
            }

            if (line.hasOption(startRecord.getOpt())) {
                server.recordFile = (File)line.getParsedOptionValue(startRecord.getOpt());
                logger.info("start recording to: {}", server.recordFile);
            }

            if (line.hasOption(startPlay.getOpt())) {
                server.playFile = (File)line.getParsedOptionValue(startPlay.getOpt());
                logger.info("start playing from: {}", server.playFile);
            }

            logger.info("starting server");
            server.start();

        } catch (ParseException ex) {
            logger.error("Unexpected exception", ex);
            usage(options);
        } catch (Exception ex) {
            logger.error("Exception", ex);
        }
    }

}
