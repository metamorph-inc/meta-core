package edu.vanderbilt.isis.meta.link;

import com.google.protobuf.TextFormat;
import edu.vanderbilt.isis.meta.MetaLinkMsg;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * User: Fred Eisele
 * Date: 6/4/13
 * Time: 4:20 PM
 */
public enum SampleInterestMsg {
    INSTANCE;
    private static final Logger logger = LoggerFactory
            .getLogger(SampleInterestMsg.class);

    private final MetaLinkMsg.Edit message;
    private SampleInterestMsg() {
        this.message =
                MetaLinkMsg.Edit.newBuilder()
                        .setEditMode(MetaLinkMsg.Edit.EditMode.INTEREST)
                        .addTopic("ISIS.METALINK.CADASSEMBLY")
                        .addTopic("default subtopic")
                        .build();
    }


    public String asString() {
        return TextFormat.printToString(this.message);
    }


    public MetaLinkMsg.Edit asMessage() {
        return this.message;
    }
}
