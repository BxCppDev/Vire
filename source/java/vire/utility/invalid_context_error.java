package vire.utility;

import java.io.*;
import vire.utility.base_error;

public class invalid_context_error
    extends base_error
{

    public invalid_context_error()
    {
	super();
	return;
    }

    public invalid_context_error(int code_, String message_)
    {
	super(code_, message_);
	return;
    }

    @Override
    public String get_type_id()
    {
	return "vire::utility::invalid_context_error";
    }

    @Override
    public String get_protobuf_type_id()
    {
	return "vire.utility.InvalidContextError";
    }

    public static void main(String[] args)
    {
	int code = vire.utility.error_code.EC_MAXIMUM_SYSTEM.code() + 42;
	String message = "Vire is out of order!";
	invalid_context_error ice = new invalid_context_error(code, message);
	System.out.printf("Error : %s%n", ice);
    }

}
