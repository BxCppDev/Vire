package vire.utility;

import java.io.*;
import vire.utility.base_error;

public class invalid_setup_id_error
    extends base_error
{
    String _invalid_setup_name_;    ///< Invalid setup name
    String _invalid_setup_version_; ///< Invalid setup version representation

    static int EC_INVALID_NAME    = vire.utility.error_code.EC_MAXIMUM_SYSTEM.code() + 1;
    static int EC_INVALID_VERSION = EC_INVALID_NAME + 1;

    @Override
    protected boolean _code_is_valid(int code_)
    {
	if (code_ == EC_INVALID_NAME) return true;
	if (code_ == EC_INVALID_VERSION) return true;
	return false;
    }

    public invalid_setup_id_error()
    {
	super();
	this._invalid_setup_name_ = "";
	this._invalid_setup_version_ = "";
	return;
    }

    public invalid_setup_id_error(int code_,
				  String bad_token_)
    {
	super();
	this._invalid_setup_name_ = "";
	this._invalid_setup_version_ = "";
	if (code_ == EC_INVALID_NAME) {
	    set_invalid_setup_name(bad_token_);
	} else if (code_ == EC_INVALID_VERSION) {
	    set_invalid_setup_version(bad_token_);
	}
	return;
    }

    boolean has_invalid_setup_name()
    {
	return !this._invalid_setup_name_.isEmpty();
    }

    void set_invalid_setup_name(String bad_name_)
    {
	if (!this.has_message_format()) {
	    this.set_message_format("Invalid setup name = '%n'");
	}
	this.set_code(EC_INVALID_NAME);
	this._invalid_setup_version_ = "";
	this._invalid_setup_name_ = bad_name_;
	return;
    }

    String get_invalid_setup_name()
    {
	return this._invalid_setup_name_;
    }

    void reset_invalid_setup_name()
    {
	this._invalid_setup_name_ = "";
 	return;
    }

    boolean has_invalid_setup_version()
    {
	return !this._invalid_setup_version_.isEmpty();
    }

    void set_invalid_setup_version(String bad_version_)
    {
	if (!this.has_message_format()) {
	    this.set_message_format("Invalid setup version = '%v'");
	}
	this.set_code(EC_INVALID_VERSION);
	this._invalid_setup_name_= "";
	this._invalid_setup_version_ = bad_version_;
	return;
    }

    String get_invalid_setup_version()
    {
	return this._invalid_setup_version_;
    }

    void reset_invalid_setup_version()
    {
	this._invalid_setup_version_ = "";
 	return;
    }

    @Override
    protected String _build_message()
    {
	String msg = get_message_format();
	if (has_invalid_setup_name()) {
	    msg = msg.replace("%n", _invalid_setup_name_);
	} else if (has_invalid_setup_version()) {
	    msg = msg.replace("%v", _invalid_setup_version_);
	}
	return msg;
    }

    @Override
    public String get_type_id()
    {
	return "vire::utility::invalid_setup_id_error";
    }

    @Override
    public String get_protobuf_type_id()
    {
	return "vire.utility.InvalidSetupIdError";
    }

    public static void main(String[] args)
    {
	{
	    String bad_name = "Atlas";
	    invalid_setup_id_error isie =
		new invalid_setup_id_error(invalid_setup_id_error.EC_INVALID_NAME,
					   bad_name);
	    isie.set_message_format("Setup name '%n' is invalid!");
	    System.out.printf("Error : %s%n", isie);
	}

	{
	    String bad_version = "3.14159";
	    invalid_setup_id_error isie =
		new invalid_setup_id_error(invalid_setup_id_error.EC_INVALID_VERSION,
					   bad_version);
	    isie.set_message_format("Setup version '%v' is invalid!");
	    System.out.printf("Error : %s%n", isie);
	}
    }
}
