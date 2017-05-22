package vire.message;

import java.io.*;
import java.time.Instant;
import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Optional;
import vire.protobufable_interface;
import vire.tree_dumpable_interface;
import vire.message.message_identifier;
import vire.utility.model_identifier;
import vire.utility.metadata_record;

public final class message_header
    implements vire.protobufable_interface, vire.tree_dumpable_interface
{

    message_identifier  _message_id_;
    java.time.Instant   _timestamp_;
    Optional<message_identifier> _in_reply_to_;
    boolean             _asynchronous_;
    String              _async_address_;
    vire.utility.model_identifier _body_layout_id_;
    List<vire.utility.metadata_record> _metadata_;

    public message_header()
    {
	_timestamp_ = java.time.Instant.MIN;
	_in_reply_to_ = Optional.ofNullable(null);
	_asynchronous_ = false;
	_async_address_ = "";
	_metadata_ = new ArrayList<vire.utility.metadata_record>();
	return;
    }

    public void reset()
    {
	_message_id_.reset();
	_timestamp_ = java.time.Instant.MIN;
	if (_in_reply_to_.isPresent()) {
	    _in_reply_to_.get().reset();
	}
	_async_address_ = "";
	_asynchronous_ = false;
	_body_layout_id_.reset();
    }

    public boolean is_valid()
    {
      if (! _message_id_.is_valid()) return false;
      if (_timestamp_ == java.time.Instant.MIN) return false;
      if (_in_reply_to_.isPresent()) {
	  if (!_in_reply_to_.get().is_valid()) return false;
      }
      if (! _body_layout_id_.is_valid()) return false;
      if (_asynchronous_ && _async_address_.length() == 0) return false;
      return true;
    }

    public void set_message_id(message_identifier id_)
    {
	_message_id_ = id_;
	return;
    }

    public message_identifier get_message_id()
    {
	return _message_id_;
    }

    public void set_body_layout_id(vire.utility.model_identifier id_)
    {
	_body_layout_id_ = id_;
    }

    public vire.utility.model_identifier get_body_layout_id()
    {
	return _body_layout_id_;
    }

    public boolean has_timestamp()
    {
	return _timestamp_ != java.time.Instant.MIN;
    }

    public void set_timestamp(java.time.Instant ts_)
    {
	_timestamp_ = ts_;
	return;
    }

    public java.time.Instant get_timestamp()
    {
	return _timestamp_;
    }

    public boolean has_in_reply_to()
    {
	if (_in_reply_to_.isPresent()) return false;
	return _in_reply_to_.get().is_valid();
    }

    public void set_in_reply_to(message_identifier id_)
    {
	_in_reply_to_ = Optional.of(id_);
	return;
    }

    public message_identifier get_in_reply_to()
    {
	return _in_reply_to_.get();
    }

    public boolean is_asynchronous()
    {
	return _asynchronous_;
    }

    public void set_asynchronous(boolean a_)
    {
	_asynchronous_ = a_;
	return;
    }

    public boolean has_async_address()
    {
	return _async_address_.length() < 0;
    }

    public void set_async_address(String aa_)
    {
	_async_address_ = aa_;
	return;
    }

    public String get_async_address()
    {
	return _async_address_;
    }

    @Override
    public String get_type_id()
    {
	return "vire::message::message_header";
    }

    @Override
    public String get_protobuf_type_id()
    {
	return "vire.message.MessageHeader";
    }

    public void add_metadata(String key_, String value_)
    {
	_metadata_.add(new metadata_record(key_, value_));
    }

    public boolean has_metadata(String key_)
    {
	Iterator<metadata_record> iterator = _metadata_.iterator();
	while (iterator.hasNext()){
	   metadata_record mdr = iterator.next();
	   if (mdr.get_key().equals(key_)) {
	       return true;
	   }
	}
	return false;
    }

    public String get_metadata_value(String key_)
	throws Exception
    {
	Iterator<metadata_record> iterator = _metadata_.iterator();
	while (iterator.hasNext()){
	   metadata_record mdr = iterator.next();
	   if (mdr.get_key().equals(key_)) {
	       return mdr.get_value();
	   }
	}
	throw new Exception("No metadata with key '" + key_ + "'!");
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

	out.println(indent_ + vire.tree_dumpable_interface.tag()
		  + "Message ID : " + _message_id_);

	out.println(indent_ + vire.tree_dumpable_interface.tag()
		  + "Timestamp : " + _timestamp_);

	out.print(indent_ + vire.tree_dumpable_interface.tag()
		  + "In reply to : ");
	if (_in_reply_to_.isPresent()) {
	    out.print(_in_reply_to_.get());
	} else {
	    out.print("<none>");
	}
	out.println();

	out.println(indent_ + vire.tree_dumpable_interface.tag()
		  + "Asynchronous : " + _asynchronous_);

	out.println(indent_ + vire.tree_dumpable_interface.tag()
		  + "Body layout ID : " + _body_layout_id_);

	out.println(indent_ + vire.tree_dumpable_interface.tag()
		    + "Metadata : [" + _metadata_.size() + ']');

	Iterator<metadata_record> iterator = _metadata_.iterator();
	while (iterator.hasNext()){
	   metadata_record mdr = iterator.next();
	   out.print(indent_ + vire.tree_dumpable_interface.skip_tag());
	   if (iterator.hasNext()) {
	       out.print(vire.tree_dumpable_interface.tag());
	   } else {
	       out.print(vire.tree_dumpable_interface.last_tag());
	   }
	   out.println("Metadata = " + mdr);
	}

	out.println(indent_ + vire.tree_dumpable_interface.last_tag()
		  + "Validity : " + is_valid());

	return;
    }

    public static void main(String[] args)
    {
	java.time.Instant timestamp = java.time.Instant.now();
	timestamp.plusNanos(123456789);

	message_header mh = new message_header();
	message_identifier msg_id = new message_identifier("vire.client.0", 23);
	// The default body layout:
	vire.utility.model_identifier body_layout_id =
	    new vire.utility.model_identifier("vire::message::body_format::typed_payload", "1.0");
	mh.set_message_id(msg_id);
	mh.set_timestamp(timestamp);
	mh.set_body_layout_id(body_layout_id);
	mh.set_asynchronous(false);
	mh.add_metadata("test", "John");
	mh.add_metadata("test1", "Foo");
	mh.add_metadata("test2", "Bar");
        mh.tree_dump(System.out, "Message header: ", "", false);
	return;
    }

}
