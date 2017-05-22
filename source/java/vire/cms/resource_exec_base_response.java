package vire.cms;

import java.io.*;
import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;
import vire.utility.base_response;
import vire.cms.method_argument;
import vire.cms.resource_status_record;

public class resource_exec_base_response
    extends base_response
{

    resource_status_record _status_record_;

    public resource_exec_base_response()
    {
	super();
	return;
    }

    public void reset()
    {
	_status_record_ = null;
	return;
    }

    public boolean has_status_record()
    {
	if (_status_record_ == null) return false;
	return true;
    }

    public resource_status_record get_status_record()
    {
	return _status_record_;
    }

    public void set_status_record(resource_status_record rsr_)
    {
	_status_record_ = rsr_;
	return;
    }

    @Override
    public String get_type_id()
    {
	return "vire::cms::resource_exec_base_response";
    }

    @Override
    public String get_protobuf_type_id()
    {
	return "vire.cms.ResourceExecBaseResponse";
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

	out.print(indent_ + vire.tree_dumpable_interface.inherit_tag(inherits_)
		  + "Status record : ");
	if (!has_status_record()) {
	    out.print("<none>");
	}
	out.println();
	if (has_status_record()) {
	    _status_record_.tree_dump(out_, "",
				      indent_
				      + vire.tree_dumpable_interface.inherit_skip_tag(inherits_),
				      false);
	}

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

	resource_exec_base_response resp = new resource_exec_base_response();
	resp.set_status_record(rsr);
	resp.tree_dump(System.out, "Resource exec base response: ", "", false);
    }

}
