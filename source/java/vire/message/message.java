package vire.message;

import java.io.*;
import vire.protobufable_interface;
import vire.tree_dumpable_interface;
import vire.message.message_header;
import vire.message.message_body;
import vire.utility.base_alarm;

public final class message
    implements vire.protobufable_interface, vire.tree_dumpable_interface
{
    message_header _header_; //!< Message header
    message_body   _body_;   //!< Message body

    public message()
    {
	return;
    }

    public void reset()
    {
	_header_ = null;
	_body_ = null;
	return;
    }

    public boolean is_valid()
    {
	if (_header_ == null || !_header_.is_valid()) return false;
	if (_body_ == null || !_body_.is_valid()) return false;
	return true;
    }

    boolean has_header()
    {
	if (_header_ == null) return false;
	return true;
    }

    void set_header(message_header header_) throws Exception
    {
	if (!header_.is_valid()) {
	    throw new Exception("Invalid header!");
	}
	_header_ = header_;
	return;
    }

    message_header get_header()
    {
	return _header_;
    }


    boolean has_body()
    {
	if (_body_ == null) return false;
	return true;
    }

    void set_body(message_body body_) throws Exception
    {
	if (!body_.is_valid()) {
	    throw new Exception("Invalid body!");
	}
	_body_ = body_;
	return;
    }

    message_body get_body()
    {
	return _body_;
    }

    @Override
    public String get_type_id()
    {
	return "vire::message::message";
    }

    @Override
    public String get_protobuf_type_id()
    {
	return "vire.message.Message";
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
		  + "Header : ");
	if (_header_ == null) {
	    out.print("<none>");
	}
	out.println();
	if (_header_ != null) {
	    _header_.tree_dump(out_, "", indent_ + vire.tree_dumpable_interface.skip_tag(), false);
	}

	out.print(indent_ + vire.tree_dumpable_interface.tag()
		  + "Body : ");
	if (_body_ == null) {
	    out.print("<none>");
	}
	out.println();
	if (_body_ != null) {
	    _body_.tree_dump(out_, "", indent_ + vire.tree_dumpable_interface.skip_tag(), false);
	}

	out.println(indent_ + vire.tree_dumpable_interface.last_tag()
		    + "Validity : " + is_valid());

	return;
    }

    public static void main(String[] args)
    {
	try {

	    message_header mh = new message_header();
	    message_identifier msg_id = new message_identifier("vire.client.0", 23);
	    // The default body layout:
	    vire.utility.model_identifier body_layout_id =
		new vire.utility.model_identifier("vire::message::body_format::typed_payload", "1.0");
	    mh.set_message_id(msg_id);
	    java.time.Instant message_timestamp = java.time.Instant.now();
	    mh.set_timestamp(message_timestamp);
	    mh.set_body_layout_id(body_layout_id);
	    mh.set_asynchronous(false);
	    mh.add_metadata("test", "John");
	    mh.add_metadata("test1", "Foo");
	    mh.add_metadata("test2", "Bar");
	    mh.add_metadata("PI", "3.14159");

	    message_body mb = new message_body();
	    java.time.Instant alarm_timestamp = java.time.Instant.now();
	    alarm_timestamp.plusNanos(123456789);
	    mb.set_payload(new base_alarm(alarm_timestamp, "warning", "Hello, Vire!"));

	    message m = new message();
	    m.set_header(mh);
	    m.set_body(mb);
	    m.tree_dump(System.out, "Message: ", "", false);

	} catch (Exception error) {
	    System.err.println("error: " + error.getMessage());
	    System.exit(1);
	}
	System.exit(0);
    }
}
