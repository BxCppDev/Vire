package vire.cms;

import java.io.*;
import java.time.Instant;
import vire.protobufable_interface;
import vire.tree_dumpable_interface;

public class resource_status_record
    implements vire.protobufable_interface,
	       vire.tree_dumpable_interface
{
    String  _path_;      ///< Resource path
    java.time.Instant _timestamp_; ///< Last modification timestamp
    boolean _missing_;   ///< Missing bit
    boolean _failed_;    ///< Failed bit
    boolean _pending_;   ///< Pending bit
    boolean _disabled_;  ///< Disabled bit

    public resource_status_record()
    {
	_timestamp_ = java.time.Instant.MIN;
	_missing_ = false;
	_failed_ = false;
	_pending_ = false;
	_disabled_ = false;
	return;
    }

    public resource_status_record(String path_, java.time.Instant t_)
    {
	_path_ = path_;
	_timestamp_ = t_;
	_missing_ = false;
	_failed_ = false;
	_pending_ = false;
	_disabled_ = false;
	return;
    }

    public void reset()
    {
	_path_ = "";
	_timestamp_ = java.time.Instant.MIN;
	_missing_ = false;
	_failed_ = false;
	_pending_ = false;
	_disabled_ = false;
	return;
    }

    public boolean has_path()
    {
	return !_path_.isEmpty();
    }

    public void set_path(String path_)
    {
	_path_ = path_;
	return;
    }

    public String get_path()
    {
	return _path_;
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

    boolean is_failed()
    {
	return _failed_;
    }

    public void set_failed(boolean failed_)
    {
	_failed_ = failed_;
	return;
    }

    boolean is_pending()
    {
	return _pending_;
    }

    public void set_pending(boolean pending_)
    {
	_pending_ = pending_;
	return;
    }

    boolean is_disabled()
    {
	return _disabled_;
    }

    public void set_disabled(boolean disabled_)
    {
	_disabled_ = disabled_;
	return;
    }

    boolean is_missing()
    {
	return _missing_;
    }

    public void set_missing(boolean missing_)
    {
	_missing_ = missing_;
	return;
    }

    @Override
    public String toString()
    {
	String s = '[' + get_path() + ';'
	    + get_timestamp().toString() + ';'
	    + is_missing() + ';'
	    + is_failed() + ';'
	    + is_disabled() + ';'
	    + is_pending() + ']'
	    ;
	return s;
    }

    @Override
    public String get_type_id()
    {
	return "vire::cms::resource_status_record";
    }

    @Override
    public String get_protobuf_type_id()
    {
	return "vire.cms.ResourceStatusRecord";
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
		    + "Path : '" + get_path() + "'");

	out.print(indent_ + vire.tree_dumpable_interface.tag()
		  + "Timestamp : ");
	if (! has_timestamp()) {
	    out.print("<none>");
	} else {
	    out.print("[" + get_timestamp() + ']');
	}
	out.println();

	out.println(indent_ + vire.tree_dumpable_interface.tag()
		    + "Missing  : " + is_missing());

	out.println(indent_ + vire.tree_dumpable_interface.tag()
		    + "Failed   : " + is_failed());

	out.println(indent_ + vire.tree_dumpable_interface.tag()
		    + "Disabled : " + is_disabled());

	out.println(indent_ + vire.tree_dumpable_interface.inherit_tag(inherits_)
		    + "Pending  : " + is_pending());

	return;
    }

    public static void main(String[] args)
    {
	java.time.Instant timestamp = java.time.Instant.now();
	timestamp.plusNanos(123456789);
	resource_status_record rsr = new resource_status_record();
	rsr.set_path("SuperNEMO://Demonstrator/CMS/Coil/Monitor/Voltage/__dp_read__");
	rsr.set_timestamp(timestamp);
	rsr.set_missing(false);
	rsr.set_failed(false);
	rsr.set_disabled(false);
	rsr.set_pending(true);
	rsr.tree_dump(System.out, "Resource status record: ", "", false);
	System.out.println("Status : " + rsr);
    }

}
