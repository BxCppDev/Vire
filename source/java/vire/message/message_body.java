package vire.message;

import java.io.*;

public final class message_body
    implements vire.protobufable_interface, vire.tree_dumpable_interface
{

    vire.utility.model_identifier _payload_type_id_; //!< Payload type identifier
    vire.utility.base_payload     _payload_;         //!< Handle to the payload object

    public message_body()
    {
	return;
    }

    public message_body(vire.utility.base_payload payload_)
	throws Exception
    {
	set_payload(payload_);
	return;
    }

    boolean is_valid()
    {
	if (!has_payload_type_id()) return false;
	if (!has_payload()) return false;
	return true;
    }

    void reset()
    {
	_payload_type_id_ = null;
	_payload_ = null;
	return;
    }

    boolean has_payload_type_id()
    {
	if (_payload_type_id_ == null) return false;
	return _payload_type_id_.is_valid();
    }

    vire.utility.model_identifier get_payload_type_id()
    {
      return _payload_type_id_;
    }

    void set_payload_type_id(vire.utility.model_identifier id_)
    {
      _payload_type_id_ = id_;
      return;
    }

    boolean has_payload()
    {
	if (_payload_ == null) return false;
	return true;
    }

    void remove_payload()
    {
	_payload_ = null;
	_payload_type_id_.reset();
	return;
    }

    void set_payload(vire.utility.base_payload payload_)
	throws Exception
    {
	if (has_payload()) {
	    remove_payload();
	}
	_payload_ = payload_;
	vire.utility.model_identifier id
	    = new vire.utility.model_identifier();
        id.set_name(_payload_.get_type_id());
        set_payload_type_id(id);
	return;
    }

    vire.utility.base_payload get_payload()
    {
	return _payload_;
    }

    @Override
    public String get_type_id()
    {
	return "vire::message::message_body";
    }

    @Override
    public String get_protobuf_type_id()
    {
	return "vire.message.MessageBody";
    }

    @Override
    public void tree_dump(OutputStream out_,
			  String title_,
			  String indent_,
			  boolean inherits_)
    {
	PrintStream out = new PrintStream(out_);
	if (!title_.isEmpty()) {
	    out.println(indent_ + title_);
	}

	out.print(indent_ + vire.tree_dumpable_interface.tag()
		  + "Payload type ID : ");
	if (has_payload_type_id()) {
	    out.print(_payload_type_id_);
	} else {
	    out.print("<none>");
	}
	out.println();

	out.print(indent_ + vire.tree_dumpable_interface.tag()
		  + "Payload ID : ");
	if (! has_payload()) {
	    out.print("<none>");
	}
	out.println();
	if (has_payload()) {
	    _payload_.tree_dump(out_,
				"",
				indent_ + vire.tree_dumpable_interface.skip_tag(),
				false);
	}

	out.println(indent_ + vire.tree_dumpable_interface.last_tag()
		  + "Validity : " + is_valid());

	return;
    }

    public static void main(String[] args)
    {
	try {

	    java.time.Instant timestamp = java.time.Instant.now();
	    timestamp.plusNanos(123456789);
	    String severity = "information";
	    String message = "Hello, Vire!";
	    vire.utility.base_alarm a =
		new vire.utility.base_alarm(timestamp, severity, message);

	    message_body mb = new message_body();
	    mb.set_payload(a);
	    mb.tree_dump(System.out, "Message body: ", "", false);
	    System.out.println("The end.");
	} catch (Exception error) {
	    System.err.println("error: " + error.getMessage());
	    System.exit(1);
	}
	System.exit(0);
    }

}
