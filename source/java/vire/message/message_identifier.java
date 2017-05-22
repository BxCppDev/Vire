package vire.message;

import java.io.*;
import vire.protobufable_interface;

public class message_identifier
    implements vire.protobufable_interface
{

    String _emitter_; //!< Name of the emitter
    int    _number_;  //!< Message unique number with respect from the emitter

    static int INVALID_NUMBER = -1;

    public message_identifier()
    {
	_emitter_ = "";
	_number_ = INVALID_NUMBER;
	return;
    }

    public message_identifier(String emitter_, int number_)
    {
	set_emitter(emitter_);
	set_number(number_);
	return;
    }

    void set_emitter(String name_)
    {
	_emitter_ = name_;
	return;
    }

    String get_emitter()
    {
	return _emitter_;
    }

    void set_number(int n_)
    {
	_number_ = n_;
	return;
    }

    int get_number()
    {
	return _number_;
    }

    void reset()
    {
	_emitter_ = "";
	_number_  = INVALID_NUMBER;
	return;
    }

    boolean is_valid()
    {
	if (_emitter_.isEmpty()) return false;
	if (_number_ == INVALID_NUMBER) return false;
	return true;
    }

    @Override
    public String toString()
    {
	String s = "[emitter=" + this.get_emitter()
	    + ";number=" + Integer.toString(_number_) + ']';
	return s;
    }

    @Override
    public String get_type_id()
    {
	return "vire::message::message_identifier";
    }

    @Override
    public String get_protobuf_type_id()
    {
	return "vire.message.MessageIdentifier";
    }

    public static void main(String[] args)
    {
	try {
	    System.err.println("Test 'vire.message.message_identifier' class!");
	    message_identifier mi = new message_identifier("vire.client.3", 42);
	    System.out.println("Message identifier = " + mi);
	    System.exit(0);
	} catch (Exception e) {
	    e.printStackTrace();
	    System.exit(1);
	}

    }

}
