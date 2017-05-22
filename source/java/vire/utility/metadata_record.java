package vire.utility;

import java.io.*;
import vire.protobufable_interface;

public class metadata_record
    implements protobufable_interface
{

    private String _key_;   ///< Metadata key
    private String _value_; ///< Metadata value representation

    public metadata_record()
    {
	return;
    }

    public metadata_record(String key_, String value_)
    {
	this._key_ = key_;
	this._value_ = value_;
	return;
    }

    @Override
    public String get_type_id()
    {
	return "vire::utility::metadata_record";
    }

    @Override
    public String get_protobuf_type_id()
    {
	return "vire.utility.MetadataRecord";
    }

    public boolean has_key()
    {
	return this._key_.length() > 0;
    }

    public void set_key(String key_)
    {
	this._key_ = key_;
    }

    public String get_key()
    {
	return this._key_;
    }

    public void set_value(String value_)
    {
	this._value_ = value_;
    }

    public boolean has_value()
    {
	return this._value_.length() > 0;
    }

    public String get_value()
    {
	return this._value_;
    }

    @Override
    public String toString()
    {
        return "[key='" + get_key() + "';value='" + get_value() + "']";
    }

    public static void main(String[] args)
    {
	try {
	    System.err.println("Test 'vire.utility.metadata_record' class!");
	    metadata_record mdr = new metadata_record();
	    mdr.set_key("name");
	    mdr.set_value("joe");
	    System.out.println("Metadata record = " + mdr);
	    System.exit(0);
	} catch (Exception e) {
	    e.printStackTrace();
	    System.exit(0);
	}

    }
}
