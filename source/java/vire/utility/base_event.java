package vire.utility;

import java.io.*;
import vire.utility.base_payload;
import java.time.Instant;

/// \brief Base event payload
public class base_event
    extends base_payload
{

    private java.time.Instant _timestamp_; //!< Event timestamp	 (MIN is considered as invalid)

    public base_event()
    {
	// Default: MIN is considered as invalid
	_timestamp_ = java.time.Instant.MIN;
	return;
    }

    public base_event(java.time.Instant ts_)
    {
	set_timestamp(ts_);
	return;
    }

    @Override
    public String get_type_id()
    {
	return "vire::utility::base_event";
    }

    @Override
    public String get_protobuf_type_id()
    {
	return "vire.utility.BaseEvent";
    }

    @Override
    public final payload_category get_category()
    {
	return payload_category.CATEGORY_EVENT;
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

    public void reset_timestamp()
    {
	_timestamp_ = java.time.Instant.MIN;
	return;
    }

    @Override
    public String toString()
    {
	String s = '[' + get_timestamp().toString() +']';
	return s;
    }

    @Override
    public void tree_dump(OutputStream out_,
			  String title_,
			  String indent_,
			  boolean inherits_)
    {
	super.tree_dump(out_, title_, indent_, true);

	PrintStream out = new PrintStream(out_);

	out.print(indent_ + vire.tree_dumpable_interface.inherit_tag(inherits_)
		  + "Timestamp : ");
	if (! has_timestamp()) {
	    out.print("<none>");
	} else {
	    out.print("[" + get_timestamp() + ']');
	}
	out.println();

	return;
    }

    public static void main(String[] args)
    {
	java.time.Instant timestamp = java.time.Instant.now();
	timestamp.plusNanos(123456789);
	base_event e = new base_event(timestamp);
	if (e.has_timestamp()) {
	    System.out.printf("Base event : %s%n", e);
	}
	e.tree_dump(System.out, "Base event: ", "", false);
	return;
    }

}
