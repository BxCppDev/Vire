package vire.cms;

import java.io.*;
import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;
import vire.utility.base_request;
import vire.cms.method_argument;

public class resource_exec_request
    extends base_request
{

    String _path_; ///< Resource path/identifier
    List<method_argument> _input_arguments_; ///< List of input arguments

    public resource_exec_request()
    {
	_input_arguments_ = new ArrayList<method_argument>();
	return;
    }

    public boolean has_path()
    {
	return ! _path_.isEmpty();
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

    public boolean has_input_arguments()
    {
	return _input_arguments_.size() > 0;
    }

    public boolean has_input_argument(String name_)
    {
	Iterator<method_argument> iterator = _input_arguments_.iterator();
	while (iterator.hasNext()){
	   method_argument arg = iterator.next();
	   if (arg.get_name().equals(name_)) {
	       return true;
	   }
	}
	return false;
    }

    public void add_input_argument(String name_, String value_repr_)
	throws Exception
    {
	if (has_input_argument(name_)) {
	    throw new Exception("Input argument named '" + name_ + "' already exists!");
	}
	_input_arguments_.add(new method_argument(name_, value_repr_));
	return;
    }

    public void add_input_argument(String name_, String value_repr_, String meta_)
	throws Exception
    {
	if (has_input_argument(name_)) {
	    throw new Exception("Input argument named '" + name_ + "' already exists!");
	}
	_input_arguments_.add(new method_argument(name_, value_repr_, meta_));
	return;
    }

    public int get_number_of_input_arguments()
    {
	return _input_arguments_.size();
    }

    method_argument get_input_argument(int index_)
    {
	return _input_arguments_.get(index_);
    }

    void clear_input_argument()
    {
	_input_arguments_.clear();
	return;
    }

    @Override
    public String get_type_id()
    {
	return "vire::cms::resource_exec_request";
    }

    @Override
    public String get_protobuf_type_id()
    {
	return "vire.cms.ResourceExecRequest";
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
		  + "Path : ");
	if (! has_path()) {
	    out.print("<none>");
	} else {
	    out.print("'" + get_path() + "'");
	}
	out.println();

	out.print(indent_ + vire.tree_dumpable_interface.inherit_tag(inherits_)
		  + "Input arguments : ");
	if (! has_input_arguments()) {
	    out.print("<none>");
	} else {
	    out.print("[" + get_number_of_input_arguments() + "]");
	}
	out.println();
	Iterator<method_argument> iterator = _input_arguments_.iterator();
	while (iterator.hasNext()){
	    method_argument arg = iterator.next();
	    out.print(indent_ + vire.tree_dumpable_interface.inherit_skip_tag(inherits_));
	    if (iterator.hasNext()) {
		out.print(vire.tree_dumpable_interface.tag());
	    } else {
		out.print(vire.tree_dumpable_interface.last_tag());
	    }
	   out.println("Input argument = " + arg);
	}

	return;
    }

    public static void main(String[] args)
    {
	try {
	    resource_exec_request rer = new resource_exec_request();
	    rer.set_path("SuperNEMO:/Demonstrator/CMS/Coil/Control/Voltage/__dp_write__");
	    rer.add_input_argument("value", "503.4 V");
	    rer.add_input_argument("dummy", "foo");
	    rer.tree_dump(System.out, "Resource exec request: ", "", false);
	} catch (Exception error) {
	    System.err.println("error: " + error.getMessage());
	    System.exit(1);
	}
	System.exit(0);
    }

}
