#include <gtkmm.h>
#include <iostream>
#include "Package.h"
#include "Encoder.h"

void encode_button_clicked(Glib::RefPtr<Gtk::Builder> builder)
{
  Gtk::ComboBoxText* gf_widget;
  Gtk::ComboBoxText* prim_poly_widget;
  Gtk::ComboBoxText* gen_poly_widget;
  Gtk::TextView* msg_widget;
  builder->get_widget("GF Size", gf_widget);
  builder->get_widget("Prim Poly", prim_poly_widget);
  builder->get_widget("Gen Poly", gen_poly_widget);
  builder->get_widget("msg", msg_widget);

  string gfStr = gf_widget->get_active_text();
  string primpolyStr = prim_poly_widget->get_active_id() ;
  string genpolyStr = gen_poly_widget->get_active_id();
  string msgStr = msg_widget->get_buffer()->get_text();

  Encoder encoder = Encoder();
  Package package = encoder.setup(gfStr, primpolyStr, genpolyStr, msgStr);

}


int main(int argc, char *argv[])
{
  auto app =
    Gtk::Application::create(argc, argv,
      "org.gtkmm.examples.base");

  Gtk::Window* window;
  Gtk::Button* encodeButton;
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("ui1.glade");
  builder->get_widget("mainwin", window);
  builder->get_widget("encode", encodeButton);
  encodeButton->signal_clicked().connect(sigc::bind<Glib::RefPtr<Gtk::Builder>>(sigc::ptr_fun(&encode_button_clicked), builder));

  return app->run(*window);
}
