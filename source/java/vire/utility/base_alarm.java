package vire.utility;

import java.io.*;
import vire.utility.base_event;

/// \brief Base alarm event
///
/// Contains a severity and a simple message
/// Severity is an arbitrary character string (example: "warning", "fatal"...)
public class base_alarm
    extends base_event
{

    String _severity_; //!< Alarm severity
    String _message_;  //!< Alarm message

    public base_alarm()
    {
	super();
	return;
    }

    public base_alarm(java.time.Instant ts_,
		      String severity_,
		      String message_)
    {
	super(ts_);
	set_severity(severity_);
	set_message(message_);
	return;
    }

    @Override
    public String get_type_id()
    {
	return "vire::utility::base_alarm";
    }

    @Override
    public String get_protobuf_type_id()
    {
	return "vire.utility.BaseAlarm";
    }

    public boolean has_severity()
    {
	return !_severity_.isEmpty();
    }

    public void set_severity(String severity_)
    {
	_severity_ = severity_;
	return;
    }

    public String get_severity()
    {
	return _severity_;
    }

    public void reset_severity()
    {
	_severity_ = "";
	return;
    }

    public boolean has_message()
    {
	return !_message_.isEmpty();
    }

    public void set_message(String message_)
    {
	_message_ = message_;
	return;
    }

    public String get_message()
    {
	return _message_;
    }

    public void reset_message()
    {
	_message_ = "";
	return;
    }

    @Override
    public String toString()
    {
	String s = '[' + get_timestamp().toString()
	    + ':' + get_severity() + "] "
	    + get_message();
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
	out.print(indent_ + vire.tree_dumpable_interface.tag()
		  + "Severity : ");
	if (! has_severity()) {
	    out.print("<none>");
	} else {
	    out.print("'" + get_severity() + "'");
	}
	out.println();

	out.print(indent_ + vire.tree_dumpable_interface.inherit_tag(inherits_)
		  + "Message : ");
	if (! has_message()) {
	    out.print("<none>");
	} else {
	    out.print("'" + get_message() + "'");
	}
	out.println();

	return;
    }

    public static void main(String[] args)
    {
	java.time.Instant timestamp = java.time.Instant.now();
	timestamp.plusNanos(123456789);
	String severity = "information";
	String message = "Hello, Vire!";
	base_alarm a = new base_alarm(timestamp, severity, message);
	System.out.printf("Base alarm : %s%n", a);
 	a.tree_dump(System.out, "Base alarm: ", "", false);
	return;
    }

}
