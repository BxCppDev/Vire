package vire.utility;

import java.io.*;
import vire.protobufable_interface;

public class base_error implements protobufable_interface
{
    private int    _code_;           ///< Error code
    private String _message_format_; ///< Raw message format
    private String _cached_message_; ///< Not serialized

    public base_error()
    {
	this._code_ = error_code.EC_GENERIC_FAILURE.code();
	this._message_format_ = "";
	this._cached_message_ = "";
	return;
    }

    public base_error(int code_)
    {
	this._code_ = code_;
	this._message_format_ = "";
	this._cached_message_ = "";
	return;
    }

    public base_error(String message_format_)
    {
	this._code_ = error_code.EC_GENERIC_FAILURE.code();
	this._message_format_ = message_format_;
	this._cached_message_ = "";
	return;
    }

    public base_error(int code_, String message_format_)
    {
	this._code_ = code_;
	this._message_format_ = message_format_;
	this._cached_message_ = "";
	return;
    }

    @Override
    public String get_type_id()
    {
	return "vire::utility::base_error";
    }

    @Override
    public String get_protobuf_type_id()
    {
	return "vire.utility.BaseError";
    }

    public boolean is_success()
    {
	return this._code_ == 0;
    }

    public boolean is_failure()
    {
	return !this.is_success();
    }

    public void set_code(int code_)
    {
	this._code_ = code_;
	return;
    }

    public int get_code()
    {
	return this._code_;
    }

    public boolean has_message_format()
    {
	return !this._message_format_.isEmpty();
    }

    public void set_message_format(String msg_)
    {
	this._message_format_ = msg_;
	return;
    }

    public void set_message(String msg_)
    {
	this.set_message_format(msg_);
	return;
    }

    public String get_message_format()
    {
	return this._message_format_;
    }

    public void clear_message_format()
    {
	this._message_format_ = "";
	return;
    }

    public String get_message()
    {
      if (this._cached_message_.isEmpty()) {
	  if (this.has_message_format()) {
	      this._cached_message_ = this._build_message();
	  }
      }
      return this._cached_message_;
    }

    public boolean has_message()
    {
      if (!this._cached_message_.isEmpty()) return true;
      if (!this._message_format_.isEmpty()) return true;
      return false;
    }

    public void reset()
    {
	this._code_ = error_code.EC_GENERIC_FAILURE.code();
	this._message_format_ = "";
	this._cached_message_ = "";
	return;
    }


    public void make_failure(int code_,
			     String message_format_)
    {
	this.set_code(code_);
	this.set_message_format(message_format_);
	return;
    }

    public void make_success(String message_format_)
    {
	this.set_code(error_code.EC_SUCCESS.code());
	this.set_message_format(message_format_);
	return;
    }

    // Default behaviour
    protected boolean _code_is_valid(int code_)
    {
	return true;
    }

    // Default behaviour: return the raw message format (without substitution)
    protected String _build_message()
    {
	return _message_format_;
    }

    @Override
    public String toString()
    {
	String s = "[code=" + this.get_code() + ']';
	if (this.has_message()) {
	    s += " " + this.get_message();
	}
	return s;
    }

    public static void main(String[] args)
    {
	try {
	    System.err.println("Test 'vire.utility.base_error' class!");
	    base_error mdr = new base_error();
	    mdr.set_code(error_code.EC_MAXIMUM_SYSTEM.code() + 14);
	    mdr.set_message_format("A dummy error");
	    System.out.println("Base error = " + mdr);
	    System.exit(0);
	} catch (Exception e) {
	    e.printStackTrace();
	    System.exit(1);
	}

    }
}
