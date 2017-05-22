package vire.utility;

/// \brief List of system error codes
///
/// Vire convention: error codes  specific to user applications and/or
/// classes must start at 100 (= EC_MAXIMUM_SYSTEM + 1)
///
public enum error_code
{
    EC_SUCCESS         (0,  "success"),
    EC_GENERIC_FAILURE (1,  "generic_failure"),
    EC_MAXIMUM_SYSTEM  (99, "maximum_system");

    private final int    _code_;
    private final String _label_;

    error_code(int code_, String label_)
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
        for (error_code ec : error_code.values()) {
	    System.out.printf("Error code with label '%s' has code: %d%n",
			      ec.label(), ec.code());
	}
    }

}
