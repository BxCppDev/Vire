package vire;

// import java.io.*;

/// \brief Interface for all protobufable objects
public interface protobufable_interface
{

    /// Return the type identifier of the class (following C++ Vire convention)
    ///
    /// Example:
    /// \code
    /// "vire::utility::base_error"
    /// \endcode
    public String get_type_id();

    /// Return the Protobuf type identifier of the class (following Protobuf convention)
    ///
    /// Example:
    /// \code
    /// "vire.utility.BaseError"
    /// \endcode
    public String get_protobuf_type_id();

    /// Not used yet
    // void protobufize(...);

    /// Not used yet
    // void deprotobufize(...);

}
