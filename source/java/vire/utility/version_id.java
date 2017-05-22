package vire.utility;

import java.io.*;
import vire.protobufable_interface;

public class version_id
    implements protobufable_interface
{

    private int    _major_;      ///<
    private int    _minor_;      ///<
    private int    _revision_;   ///<
    private String _tag_;        ///<
    private int    _tag_number_; ///<

    public static final INVALID_ID = -1;

    public version_id()
    {
	return;
    }

    @Override
    public String get_type_id()
    {
	return "vire::utility::version_id";
    }

    @Override
    public String get_protobuf_type_id()
    {
	return "vire.utility.VersionId";
    }

    @Override
    public String toString()
    {
        return "key='" + get_key() + "';value='" + get_value() + "']";
    }

    public static void main(String[] args)
    {
	try {
	    System.err.println("Test 'vire.utility.version_id' class!");
	    version_id vid = new version_id();
	    // vid.set_key("name");
	    System.out.println("Version ID = " + vid);
	    System.exit(0);
	} catch (Exception e) {
	    e.printStackTrace();
	    System.exit(0);
	}

    }
}
