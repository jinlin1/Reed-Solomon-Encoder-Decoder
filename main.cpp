#include <gtkmm.h>
#include <iostream>
#include "Package.h"
#include "Parse.h"
#include "Encoder.h"
#include "Decoder.h"

void encode_button_clicked(Glib::RefPtr<Gtk::Builder> builder)
{
  Gtk::Window* error_window;
  Gtk::Label* error_msg_widget;
  Gtk::ComboBoxText* gf_widget;
  Gtk::ComboBoxText* prim_poly_widget;
  Gtk::ComboBoxText* gen_poly_widget;
  Gtk::TextView* msg_widget;
  Gtk::TextView* output_widget;
  Gtk::TextBuffer* output_buffer;
  builder->get_widget("errwin", error_window);
  builder->get_widget("errmsg", error_msg_widget);
  builder->get_widget("GF Size", gf_widget);
  builder->get_widget("Prim Poly", prim_poly_widget);
  builder->get_widget("Gen Poly", gen_poly_widget);
  builder->get_widget("msg", msg_widget);
  builder->get_widget("output", output_widget);

  string gfeStr = gf_widget->get_active_text();
  string primpolyStr = prim_poly_widget->get_active_id() ;
  string genpolyStr = gen_poly_widget->get_active_id();
  string msgStr = msg_widget->get_buffer()->get_text();

  if(msgStr.empty()) {
    error_msg_widget->set_text("Message cannot be empty");
    error_window->show();
    return;
  }

  Parse parse = Parse(gfeStr, primpolyStr, genpolyStr, msgStr);

  if(parse.getMsg().size() + parse.getGenpoly().size() >= 1 <<parse.getGfe() ){
    error_msg_widget->set_text("Message contains too many symbols for the selected GF size and Generator Polynomial");
    error_window->show();
    return;
  }

  Encoder encoder = Encoder();
  Package package = encoder.encode(
           parse.getGfe(), 
           parse.getPrimpoly(), 
           parse.getGenpoly(), 
           parse.getMsg()); 
  output_widget->get_buffer()->set_text(package.getOutPolyStr());

}

void decode_button_clicked(Glib::RefPtr<Gtk::Builder> builder)
{
  Gtk::Window* error_window;
  Gtk::Label* error_msg_widget;
  Gtk::ComboBoxText* gf_widget;
  Gtk::ComboBoxText* prim_poly_widget;
  Gtk::ComboBoxText* gen_poly_widget;
  Gtk::TextView* msg_widget;
  Gtk::TextView* output_widget;
  Gtk::TextBuffer* output_buffer;
  builder->get_widget("errwin", error_window);
  builder->get_widget("errmsg", error_msg_widget);
  builder->get_widget("GF Size", gf_widget);
  builder->get_widget("Prim Poly", prim_poly_widget);
  builder->get_widget("Gen Poly", gen_poly_widget);
  builder->get_widget("msg", msg_widget);
  builder->get_widget("output", output_widget);

  string gfeStr = gf_widget->get_active_text();
  string primpolyStr = prim_poly_widget->get_active_id() ;
  string genpolyStr = gen_poly_widget->get_active_id();
  string msgStr = msg_widget->get_buffer()->get_text();

  if(msgStr.empty()) {
    error_msg_widget->set_text("Message cannot be empty");
    error_window->show();
    return;
  }

  Parse parse = Parse(gfeStr, primpolyStr, genpolyStr, msgStr);

  if(parse.getMsg().size() != pow(2, stoi(gfeStr)) -1) {
    error_msg_widget->set_text("Message contains too few symbols for the selected GF size");
    error_window->show();
    return;
  }

  Decoder decoder = Decoder();
  Package package = decoder.decode(
      parse.getGfe(), 
      parse.getPrimpoly(), 
      parse.getGenpoly(), 
      parse.getMsg());

  if(package.getSuccess()) {
    output_widget->get_buffer()->set_text(package.getOutPolyStr());
  } else {
    error_msg_widget->set_text(package.getErrorMessage());
    error_window->show();
  }

}

void change_combo_box(Glib::RefPtr<Gtk::Builder> builder) {
  Gtk::ComboBoxText* gf_widget;
  Gtk::ComboBoxText* prim_poly_widget;
  Gtk::ComboBoxText* gen_poly_widget;

  builder->get_widget("GF Size", gf_widget);
  builder->get_widget("Prim Poly", prim_poly_widget);
  builder->get_widget("Gen Poly", gen_poly_widget);

  prim_poly_widget->remove_all();
  gen_poly_widget->remove_all();
  
  if (gf_widget->get_active_text() == "3") {
    prim_poly_widget->append("1101","x³+x+1");
    gen_poly_widget->append("2,4,3,6","x⁴+3x³+1x²+2x+3");
    prim_poly_widget->set_active_id("1101");
    gen_poly_widget->set_active_id("2,4,3,6");
  }
  else if (gf_widget->get_active_text() == "4") {
    prim_poly_widget->append("10011","x⁴+x³+1");
    gen_poly_widget->append("2,4,8,3,6","x⁵+11x⁴+4x³+14x²+10x+2");
    prim_poly_widget->set_active_id("10011");
    gen_poly_widget->set_active_id("2,4,8,3,6");
  }
  else if (gf_widget->get_active_text() == "8") {
    prim_poly_widget->append("111000011","x⁸+x⁷+x⁶+x+1");
    gen_poly_widget->append("2,4,8,16,32,64","x⁶+126x⁵+197x⁴+44x³+104x²+120x+102");
    prim_poly_widget->set_active_id("111000011");
    gen_poly_widget->set_active_id("2,4,8,16,32,64");
  }
  
}

void close_window(Gtk::Window* window) {
  window->hide();
}

int main(int argc, char *argv[])
{
  auto app =
    Gtk::Application::create(argc, argv,
      "org.gtkmm.examples.base");

  Gtk::Window* window;
  Gtk::Window* errorWindow;
  Gtk::ComboBoxText* gf_widget;
  Gtk::Button* encodeButton;
  Gtk::Button* decodeButton;
  Gtk::Button* errorButton;
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("ui1.glade");
  builder->get_widget("mainwin", window);
  builder->get_widget("errwin", errorWindow);
  builder->get_widget("GF Size", gf_widget);
  builder->get_widget("encode", encodeButton);
  builder->get_widget("decode", decodeButton);
  builder->get_widget("errbutton", errorButton);

  gf_widget->signal_changed().connect(sigc::bind<Glib::RefPtr<Gtk::Builder>>(sigc::ptr_fun(&change_combo_box), builder));
  
  encodeButton->signal_clicked().connect(sigc::bind<Glib::RefPtr<Gtk::Builder>>(sigc::ptr_fun(&encode_button_clicked), builder));
  decodeButton->signal_clicked().connect(sigc::bind<Glib::RefPtr<Gtk::Builder>>(sigc::ptr_fun(&decode_button_clicked), builder));
  errorButton->signal_clicked().connect(sigc::bind<Gtk::Window*>(sigc::ptr_fun(&close_window), errorWindow));

  return app->run(*window);
}
