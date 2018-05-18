// i_yesno_question_dialog.cc

// Ourselves:
#include <vire/ui/i_yesno_question_dialog.h>

// Third party:
#include <bayeux/datatools/exception.h>

namespace vire {

  namespace ui {

    i_yesno_question_dialog::i_yesno_question_dialog()
    {
      return;
    }

    i_yesno_question_dialog::~i_yesno_question_dialog()
    {
      return;
    }

    bool i_yesno_question_dialog::has_default_value() const
    {
      if (_default_value_) return true;
      return false;
    }

    void i_yesno_question_dialog::set_default_value(const bool value_)
    {
      _default_value_ = value_;
      return;
    }

    bool i_yesno_question_dialog::get_default_value() const
    {
      DT_THROW_IF(!has_default_value(),
                  std::logic_error,
                  "No default value!");
      return _default_value_.value();
    }

    bool i_yesno_question_dialog::is_label_as_question() const
    {
      return _label_as_question_;
    }

    void i_yesno_question_dialog::set_label_as_question(bool laq_)
    {
      _label_as_question_ = laq_;
      return;
    }

  } // namespace ui

} // namespace vire
