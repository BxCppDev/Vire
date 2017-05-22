package vire.utility;

import java.io.*;
import vire.protobufable_interface;
import vire.utility.base_identifier;

public class instance_identifier
    extends base_identifier
	    // implements protobufable_interface
{
    public instance_identifier()
    {
	super();
	return;
    }

    public instance_identifier(String name_, String version_)
    {
	super(name_, version_);
	return;
    }

    @Override
    public String get_type_id()
    {
	return "vire::utility::instance_identifier";
    }

    @Override
    public String get_protobuf_type_id()
    {
	return "vire.utility.InstanceIdentifier";
    }

    public boolean name_is_valid(String name_)
    {
	if (name_.length() == 0) return false;
	return true;
    }

    public static void main(String[] args)
    {
	try {
	    System.err.println("Test 'vire.utility.instance_identifier' class!");
	    instance_identifier mdr = new instance_identifier();
	    mdr.set_name("name");
	    mdr.set_version("1.0");
	    System.out.println("Model identifier = " + mdr);
	    System.exit(0);
	} catch (base_identifier.invalid_name_exception e) {
	    System.out.println("Invalid name = '" + e.get_invalid_name() + "'!");
	    System.exit(1);
	} catch (Exception e) {
	    e.printStackTrace();
	    System.exit(1);
	}

    }

}
