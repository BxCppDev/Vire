package vire.utility;

import java.io.*;
import vire.protobufable_interface;

/// \brief Base identifier class
///
/// An identifier consists in:
/// - a mandatory unique name
/// - an optional version string
/// Depending on the type of identifier, some special validation
/// may occur to accept/reject the name.
/// Example:
/// \code
/// name = "SuperNEMO"
/// version = "1.0"
/// \endcode
public class base_identifier implements vire.protobufable_interface
{
    private String _name_;    ///< Name of the object (mandatory)
    private String _version_; ///< Version identifier of the object (optional)

    class invalid_name_exception extends Exception
    {
	private String _invalid_name_;

	public invalid_name_exception(String invalid_name_)
	{
	    _invalid_name_ = invalid_name_;
	    return;
	}

	public String get_invalid_name()
	{
	    return _invalid_name_;
	}
    }

    public base_identifier()
    {
	return;
    }

    public base_identifier(String name_, String version_)
    {
	this._name_ = name_;
	this._version_ = version_;
	return;
    }

    public void reset()
    {
	this._name_ = "";
	this._version_ = "";
	return;
    }

    public boolean is_valid()
    {
	if (this._name_.isEmpty()) return false;
        return true;
    }

    @Override
    public String get_type_id()
    {
	return "vire::utility::base_identifier";
    }

    @Override
    public String get_protobuf_type_id()
    {
	return "vire.utility.BaseIdentifier";
    }

    // Default behaviour: candidate name is rejected
    public boolean name_is_valid(String name_)
    {
	return false;
    }

    public boolean has_name()
    {
	return !this._name_.isEmpty();
    }

    public void set_name(String name_)
	throws invalid_name_exception
    {
	if (!name_is_valid(name_)) {
	    throw new invalid_name_exception(name_);
	}
	this._name_ = name_;
	return;
    }

    public String get_name()
    {
	return this._name_;
    }

    public void set_version(String version_)
    {
	this._version_ = version_;
    }

    public boolean has_version()
    {
	return !this._version_.isEmpty();
    }

    public String get_version()
    {
	return this._version_;
    }

    @Override
    public String toString()
    {
        return "[name='" + get_name() + "';version='" + get_version() + "']";
    }

    public static void main(String[] args)
    {
	try {
	    System.err.println("Test 'vire.utility.base_identifier' class!");
	    base_identifier mdr = new base_identifier();
	    mdr.set_name("name");
	    mdr.set_version("1.0");
	    System.out.println("Base identifier = " + mdr);
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
