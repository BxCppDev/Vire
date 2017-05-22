package vire.utility;

import java.io.*;
import vire.protobufable_interface;
import vire.tree_dumpable_interface;
import vire.utility.payload_category;

/// \brief Pure abstract payload class
public abstract class base_payload
    implements vire.protobufable_interface,
	       vire.tree_dumpable_interface
{

    public base_payload()
    {
	return;
    }

    @Override
    public String get_type_id()
    {
	return "vire::utility::base_payload";
    }

    @Override
    public String get_protobuf_type_id()
    {
	return "vire.utility.BasePayload";
    }

    /// Return the category of payload object
    public abstract payload_category get_category();

    /// Check if the payload object is a request
    public boolean is_request()
    {
	return get_category() == payload_category.CATEGORY_REQUEST;
    }

    /// Check if the payload object is a response
    public boolean is_response()
    {
	return get_category() == payload_category.CATEGORY_RESPONSE;
    }

    /// Check if the payload object is an event
    public boolean is_event()
    {
	return get_category() == payload_category.CATEGORY_EVENT;
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
		    + "Category : '" + get_category().label() + "'");

	out.println(indent_ + vire.tree_dumpable_interface.tag()
		  + "Type ID : '" + get_type_id() + "'");

	out.println(indent_ + vire.tree_dumpable_interface.inherit_tag(inherits_)
		  + "Protobuf type ID : '" + get_protobuf_type_id() + "'");

	return;
    }

}
