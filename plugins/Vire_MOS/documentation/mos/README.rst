
has_name_interface
  virtual const std::string & get_name() const = 0

has_attributes_interface
  virtual const std::vector<Attribut> & get_attributes() const = 0

has_common_interface
: has_name_interface
, has_attributes_interface


has_infos_interface
  virtual const std::vector<Info> & get_infos() const = 0

has_userinfos_interface
  virtual const std::vector<UserInfo> & get_userinfos() const = 0


has_type_interface
  virtual Type get_type() const = 0
  virtual const boost::optional<Unit> & get_unit() const = 0
