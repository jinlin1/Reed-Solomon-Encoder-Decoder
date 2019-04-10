#include <gtkmm.h>
#include <iostream>
#include "Package.h"
#include "Parse.h"
#include "Encoder.h"
#include "Decoder.h"

void encode_button_clicked(Glib::RefPtr<Gtk::Builder> builder)
{
  Gtk::ComboBoxText* gf_widget;
  Gtk::ComboBoxText* prim_poly_widget;
  Gtk::ComboBoxText* gen_poly_widget;
  Gtk::TextView* msg_widget;
  Gtk::TextView* output_widget;
  Gtk::TextBuffer* output_buffer;
  builder->get_widget("GF Size", gf_widget);
  builder->get_widget("Prim Poly", prim_poly_widget);
  builder->get_widget("Gen Poly", gen_poly_widget);
  builder->get_widget("msg", msg_widget);
  builder->get_widget("output", output_widget);

  string gfeStr = gf_widget->get_active_text();
  string primpolyStr = prim_poly_widget->get_active_id() ;
  string genpolyStr = gen_poly_widget->get_active_id();
  string msgStr = msg_widget->get_buffer()->get_text();

  Parse parse = Parse(gfeStr, primpolyStr, genpolyStr, msgStr);

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
  Gtk::ComboBoxText* gf_widget;
  Gtk::ComboBoxText* prim_poly_widget;
  Gtk::ComboBoxText* gen_poly_widget;
  Gtk::TextView* msg_widget;
  Gtk::TextView* output_widget;
  Gtk::TextBuffer* output_buffer;
  builder->get_widget("GF Size", gf_widget);
  builder->get_widget("Prim Poly", prim_poly_widget);
  builder->get_widget("Gen Poly", gen_poly_widget);
  builder->get_widget("msg", msg_widget);
  builder->get_widget("output", output_widget);

  string gfeStr = gf_widget->get_active_text();
  string primpolyStr = prim_poly_widget->get_active_id() ;
  string genpolyStr = gen_poly_widget->get_active_id();
  string msgStr = msg_widget->get_buffer()->get_text();

  Parse parse = Parse(gfeStr, primpolyStr, genpolyStr, msgStr);

  Decoder decoder = Decoder();
  Package package = decoder.decode(
      parse.getGfe(), 
      parse.getPrimpoly(), 
      parse.getGenpoly(), 
      parse.getMsg()); 

  output_widget->get_buffer()->set_text(package.getOutPolyStr());

}

int main(int argc, char *argv[])
{
  auto app =
    Gtk::Application::create(argc, argv,
      "org.gtkmm.examples.base");

  Gtk::Window* window;
  Gtk::Button* encodeButton;
  Gtk::Button* decodeButton;
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("ui1.glade");
  builder->get_widget("mainwin", window);
  builder->get_widget("encode", encodeButton);
  builder->get_widget("decode", decodeButton);
  encodeButton->signal_clicked().connect(sigc::bind<Glib::RefPtr<Gtk::Builder>>(sigc::ptr_fun(&encode_button_clicked), builder));
  decodeButton->signal_clicked().connect(sigc::bind<Glib::RefPtr<Gtk::Builder>>(sigc::ptr_fun(&decode_button_clicked), builder));

  return app->run(*window);
}
