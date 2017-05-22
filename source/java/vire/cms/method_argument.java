package vire.cms;

import java.io.*;
import vire.protobufable_interface;
import vire.tree_dumpable_interface;

public class method_argument
    implements vire.protobufable_interface,
	       vire.tree_dumpable_interface
{
    String _name_;       ///< Name of the argument
    String _value_repr_; ///< String representation of the argument's value
    String _meta_;       ///< Auxiliary metadata

    public method_argument()
    {
	return;
    }

    public method_argument(String name_, String value_repr_)
    {
	set_name(name_);
	set_value_repr(value_repr_);
	return;
    }

    public method_argument(String name_, String value_repr_, String meta_)
    {
	set_name(name_);
	set_value_repr(value_repr_);
	set_meta(meta_);
	return;
    }

    @Override
    public String get_type_id()
    {
	return "vire::cms::method_argument";
    }

    @Override
    public String get_protobuf_type_id()
    {
	return "vire.cms.MethodArgument";
    }

    public boolean is_valid()
    {
	if (!has_name()) return false;
	return true;
    }

    public boolean has_name()
    {
	if (_name_ == null) return false;
	return !_name_.isEmpty();
    }

    public void set_name(String name_)
    {
	_name_ = name_;
	return;
    }

    public void reset_name()
    {
	_name_ = "";
	return;
    }

    public String get_name()
    {
	return _name_;
    }

    public boolean has_value_repr()
    {
	if (_value_repr_ == null) return false;
	return !_value_repr_.isEmpty();
    }

    public void set_value_repr(String value_repr_)
    {
	_value_repr_ = value_repr_;
	return;
    }

    public String get_value_repr()
    {
	return _value_repr_;
    }

    public void reset_value_repr()
    {
	_value_repr_ = "";
	return;
    }

    public boolean has_meta()
    {
	if (_meta_ == null) return false;
	return !_meta_.isEmpty();
    }

    public void set_meta(String meta_)
    {
	_meta_ = meta_;
	return;
    }

    public String get_meta()
    {
	return _meta_;
    }

    public void reset_meta()
    {
	_meta_ = "";
	return;
    }


    public void reset()
    {
	reset_name();
	reset_value_repr();
	reset_meta();
	return;
    }

    @Override
    public String toString()
    {
        return "[name='" + get_name() + "';value-repr='" + get_value_repr() + "']";
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
		    + "Name : '" + get_name() + "'");

	out.println(indent_ + vire.tree_dumpable_interface.tag()
		    + "Value repr : '" + get_value_repr() + "'");

	out.print(indent_ + vire.tree_dumpable_interface.inherit_tag(inherits_)
		  + "Meta : ");
	if (has_meta()) {
	    out.print("'" + get_meta() + "'");
	} else {
	    out.print("<none>");
	}
	out.println();

	return;
    }

    public static void main(String[] args)
    {
	method_argument arg = new method_argument("voltage", "5.2 V");
 	arg.tree_dump(System.out, "Method argument: ", "", false);
    }

}
