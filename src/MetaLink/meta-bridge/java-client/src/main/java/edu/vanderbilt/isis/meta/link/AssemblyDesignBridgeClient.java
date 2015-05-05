package edu.vanderbilt.isis.meta.link;

import edu.vanderbilt.isis.meta.MetaLinkMsg;
import io.netty.bootstrap.Bootstrap;
import io.netty.buffer.ByteBuf;
import io.netty.channel.*;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioSocketChannel;
import io.netty.handler.codec.protobuf.ProtobufDecoder;
import io.netty.handler.codec.protobuf.ProtobufEncoder;
import org.apache.commons.cli.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import ch.qos.logback.classic.LoggerContext;
import ch.qos.logback.core.util.StatusPrinter;

import java.io.*;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class AssemblyDesignBridgeClient {
    private static final Logger logger = LoggerFactory
            .getLogger("metalink.client");

    private final String host;
    private final int port;
    private final BlockingQueue<MetaLinkMsg.Edit> messageQueue;
    private File recordFile;
    private Channel channel;
    private ClientMsgHandler clientMsgHandler;

    public AssemblyDesignBridgeClient(final String host, final int port) {
        this.host = host;
        this.port = port;
        this.messageQueue = new LinkedBlockingQueue<MetaLinkMsg.Edit>();
        recordFile = null;
        this.channel = null;
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
    private boolean playback(final File playbackFile) {
        logger.info("playback ");
        return SyntheticData.playback(logger, playbackFile, new SyntheticData.Deliverer() {
            final AssemblyDesignBridgeClient client = AssemblyDesignBridgeClient.this;
            @Override
            public void deliver(final MetaLinkMsg.Edit edit) {
                    client.deliver(edit);
            }
            @Override
            public String originator() {
                return "playback: " + client.channel.localAddress().toString();
            }
        });
    }

    private void deliver(final MetaLinkMsg.Edit message) {
        this.messageQueue.add(message);
        try {
            clientMsgHandler.sendPending();
        } catch (InterruptedException ex) {
            logger.warn("process interrupted");
        }
    }

    private static void usage(final Options options) {
        HelpFormatter formatter = new HelpFormatter();
        formatter.printHelp(AssemblyDesignBridgeClient.class.getSimpleName(), options);
        System.out.println("to force the loading of a specific logback.xml file:");
        System.out.println("java -Dlogback.configurationFile=/path/to/config.xml -jar ...");
    }

    private void graceful() {
        if (this.channel == null) {
            logger.warn("no active channel");
            return;
        }
        if ( ! this.channel.isActive()  ) {
            logger.warn("channel already inactive") ;
            return;
        }
        if ( ! this.channel.isOpen()  ) {
            logger.warn("channel already closed") ;
            return;
        }
        this.channel.close();
    }
    /**
     * This method does the real work.
     *
     * @throws Exception
     */
    public void run() throws Exception {
        final EventLoopGroup group = new NioEventLoopGroup();
        try {
            final Bootstrap boot = new Bootstrap();
            this.clientMsgHandler = new ClientMsgHandler(this.messageQueue, this.recordFile);

            final ChannelInitializer<SocketChannel> initializer = new ChannelInitializer<SocketChannel>() {
                final AssemblyDesignBridgeClient parent = AssemblyDesignBridgeClient.this;

                @Override
                public void initChannel(SocketChannel channel) throws Exception {
                    logger.trace("initialize socket");
                    final ChannelPipeline pipeline = channel.pipeline();

                    pipeline.addLast("frameEncoder", new MagicLengthFrameEncoder());
                    pipeline.addLast("frameDecoder", new MagicLengthFrameDecoder());

                    pipeline.addLast("protobufEncoder", new ProtobufEncoder());
                    pipeline.addLast("protobufDecoder",
                            new ProtobufDecoder(MetaLinkMsg.Edit.getDefaultInstance()));

                    // pipe.addLast("executor", foo);
                    pipeline.addLast("handler", clientMsgHandler);
                }

                @Override
                public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
                    logger.warn("cancel initializer exception");
                }
            };

            boot.group(group)
                    .channel(NioSocketChannel.class)
                    .handler(initializer);

            final ChannelFuture future = boot.connect(this.host, this.port).sync();

            final ChannelFutureListener fl = new ChannelFutureListener() {
                @Override
                public void operationComplete(ChannelFuture cf) throws Exception {
                    if (! cf.isSuccess()) {
                        logger.warn("connection failed ", cf.cause());
                        return;
                    }
                    logger.info("connection attempt succeeded");
                    AssemblyDesignBridgeClient.this.channel = future.channel();
                }
            };
            logger.info("{} started and connected to {} on {}",
                    AssemblyDesignBridgeClient.class.getSimpleName(),
                    future.channel().remoteAddress(),
                    future.channel().localAddress());

            future.addListener(fl);
            future.channel().closeFuture().sync();

        } finally {
            group.shutdownGracefully();
        }
        logger.info("run complete");
    }

    /**
     * Something to run in a separate thread.
     * It receives mode switching commands and changes the way it processes the subsequent lines.
     * The processor starts in command mode.
     * q\n : quit reading input.
     * f\n : indicates switching to reading a file path.
     *   Provide a single file path which is read and sent as a single message.
     *   Following that the processor switches back to command mode.
     * r d\n switches to read message mode
     *   Read bytes until the character 'd' is seen again.
     */
    static class InputProcessor implements Runnable {
        private  AssemblyDesignBridgeClient client;
        public InputProcessor(AssemblyDesignBridgeClient client) {
            this.client = client;
        }
        static enum Mode {
            COMMAND("^c$"),
            FILE_PATH("^f$"),
            READ_MSG("^r (\\S*)$"),
            QUIT("^q");
            public final Pattern indicator;
            private Mode(String indicator) {
                this.indicator = Pattern.compile(indicator);
            }
        }
        public void run() {
            final BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
            Mode mode = Mode.COMMAND;
            String readTerminator = "@end";
            StringBuffer workingMsg = new StringBuffer();
            while(true){
                logger.info("current mode: {}", mode);
                final String record;
                try {
                    record = br.readLine();
                } catch (IOException ex) {
                    logger.error("could not read line");
                    return;
                }
                switch( mode ) {
                    case FILE_PATH:
                    {
                        final File messageFile = new File(record);
                        logger.info("start playing from: {}", messageFile);
                        try {
                            client.playback(messageFile);
                        } finally {
                            mode = Mode.COMMAND;
                        }
                        break;
                    }
                    case READ_MSG:
                    {
                        if (readTerminator.equalsIgnoreCase(record)) {
                            try {
                                final MetaLinkMsg.Edit.Builder editBuilder = SyntheticData.decodeEdit(logger, workingMsg.toString());
                                final String originator = "client: " + client.channel.localAddress().toString();
                                editBuilder.addOrigin(originator);
                                client.deliver(editBuilder.build());
                            } finally {
                                mode = Mode.COMMAND;
                                workingMsg = new StringBuffer();
                            }
                            continue;
                        }
                        workingMsg.append(record).append('\n');
                        break;
                    }
                    case COMMAND:
                    default:
                    {
                        if (Mode.FILE_PATH.indicator.matcher(record).matches()) {
                            mode = Mode.FILE_PATH;
                            continue;
                        }
                        if (Mode.QUIT.indicator.matcher(record).matches()) {
                            logger.info("quitting client");
                            client.graceful();
                            return;
                        }
                        final Matcher isReadMsgMode = Mode.READ_MSG.indicator.matcher(record);
                        if (isReadMsgMode.matches()) {
                            mode = Mode.READ_MSG;
                            readTerminator = isReadMsgMode.group(1);
                            continue;
                        }
                        logger.warn("unknown command {}", record);
                        break;
                    }
                }
            }
        }
    }


    /**
     * This method handles the command line options.
     *
     * @param args
     * @throws Exception
     */
    public static void main(String[] args) throws Exception {
        logger.info("starting edu.vanderbilt.isis.meta.link");

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

        final Option setHost = OptionBuilder
                .withLongOpt("set-host")
                .withDescription("the name or address of the target host")
                .hasArg()
                .withArgName("HOST")
                .withType(String.class)
                .create('H');
        options.addOption(setHost);

        final Option setPort = OptionBuilder
                .withLongOpt("set-port")
                .withDescription("the port on the target host")
                .hasArg(true)
                .withArgName("PORT")
                .withType(Number.class)
                .create('P');
        options.addOption(setPort);

        final Option setPlayback = OptionBuilder
                .withLongOpt("set-playback")
                .withDescription("the message to be sent (in protobuf text format)")
                .hasArg(true)
                .withArgName("PLAYBACK")
                .withType(File.class)
                .create('p');
        options.addOption(setPlayback);


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

            if (line.hasOption("show-help")) {
                usage(options);
                return;
            }

            final String host;
            if (!(line.hasOption(setHost.getOpt()))) {
                host = "localhost";
            } else {
                host = (String)line.getParsedOptionValue(setHost.getOpt());
            }

            final int port;
            if (!(line.hasOption(setPort.getOpt()))) {
                port = 15150;
            } else {
                final Number portCandidate = (Number)line.getParsedOptionValue(setPort.getOpt());
                if (portCandidate.longValue() > Integer.MAX_VALUE)  {
                    logger.warn("port value is too large {}", portCandidate);
                    port = 15150;
                }  else if (portCandidate.longValue() < 1) {
                    logger.warn("port value is too small {}", portCandidate);
                    port = 15150;
                }  else {
                    port = portCandidate.intValue();
                }
                logger.info("setting port: {}", port);
            }

            final AssemblyDesignBridgeClient client =
                    new AssemblyDesignBridgeClient(host, port);

            final InputProcessor processor = new InputProcessor(client);
            (new Thread(processor)).start();

            logger.info("is recorder present?");
            if (line.hasOption(startRecord.getOpt())) {
                client.recordFile = (File)line.getParsedOptionValue(startRecord.getOpt());
                logger.info("start recording to: {}", client.recordFile);
            }
            logger.info("is playback present?");
            if (line.hasOption(setPlayback.getOpt())) {
                final File messageFile = (File)line.getParsedOptionValue(setPlayback.getOpt());
                if (!(messageFile.exists())) {
                    logger.error("message file is missing {}", messageFile);
                    return;
                }
                logger.info("start playing from command line: {}", messageFile);
                client.playback(messageFile) ;
            }
            logger.info("run client");
            client.run();

        } catch (ParseException ex) {
            logger.error("Unexpected exception", ex);
            usage(options);
        }
    }

}
