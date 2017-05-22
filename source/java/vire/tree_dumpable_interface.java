package vire;

import java.io.*;

public interface tree_dumpable_interface
{

    // A collection of formating tokens:

    public static String tag()
    {
	return "|-- ";
    }

    public static String skip_tag()
    {
	return "|   ";
    }

    public static String last_tag()
    {
	return "`-- ";
    }

    public static String last_skip_tag()
    {
	return "    ";
    }

    public static String inherit_tag(boolean inherits_)
    {
	if (inherits_) return tag();
	return last_tag();
    }

    public static String inherit_skip_tag(boolean inherits_)
    {
	if (inherits_) return skip_tag();
	return last_skip_tag();
    }

    /// Smart (tree-formated) printing method
    public void tree_dump(OutputStream out_,
			  String title_,
			  String indent_,
			  boolean inherits_);

}
