package vire.utility;

public enum payload_category
{
    CATEGORY_REQUEST  (1, "request"),  ///< Request RPC object
    CATEGORY_RESPONSE (2, "response"), ///< Response RPC object
    CATEGORY_EVENT    (3, "event");    ///< Event object

    private final int    _code_;
    private final String _label_;

    payload_category(int code_, String label_)
    {
	this._code_ = code_;
	this._label_ = label_;
    }

    public int code()
    {
	return this._code_;
    }

    public String label()
    {
	return this._label_;
    }

    public static void main(String[] args)
    {
        // if (args.length != 1) {
        //     System.err.println("Usage: java payload_category <label>");
        //     System.exit(-1);
        // }
        for (payload_category pc : payload_category.values()) {
	    System.out.printf("Category with label '%s' has code: %d%n",
			      pc.label(), pc.code());
	}
    }

}
