#include <chrono>
#include <gtkmm.h>
#include <iostream>
#include <fstream>
#include <sstream>
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
  Gtk::TextView* info_widget;
  Gtk::TextBuffer* output_buffer;
  Gtk::TextBuffer* info_buffer;
  builder->get_widget("errwin", error_window);
  builder->get_widget("errmsg", error_msg_widget);
  builder->get_widget("GF Size", gf_widget);
  builder->get_widget("Prim Poly", prim_poly_widget);
  builder->get_widget("Gen Poly", gen_poly_widget);
  builder->get_widget("msg", msg_widget);
  builder->get_widget("output", output_widget);
  builder->get_widget("info", info_widget);
  
  string gfeStr = gf_widget->get_active_text();
  string primpolyStr = prim_poly_widget->get_active_id() ;
  string genpolyStr = gen_poly_widget->get_active_id();
  string msgStr = msg_widget->get_buffer()->get_text();
  stringstream ss;
  
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

  auto start = chrono::steady_clock::now();
  Encoder encoder = Encoder();
  Package package = encoder.encode(
           parse.getGfe(), 
           parse.getPrimpoly(), 
           parse.getGenpoly(), 
           parse.getMsg());

  auto end = chrono::steady_clock::now();
  
  int data_size = sizeof(parse.getMsg());
  double elapsed_time_us = chrono::duration_cast<chrono::microseconds>(end - start).count();
  ss << "MSG: " << data_size << " B; Elapsed Time: " << elapsed_time_us << " µs; xfer rate: "<< ( data_size * 8 * 1000000 ) / ( elapsed_time_us * 1024 ) << " kbps";

  info_widget->get_buffer()->set_text(ss.str());  
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
  Gtk::TextView* info_widget;
  Gtk::TextBuffer* output_buffer;
  Gtk::TextBuffer* info_buffer;
  builder->get_widget("errwin", error_window);
  builder->get_widget("errmsg", error_msg_widget);
  builder->get_widget("GF Size", gf_widget);
  builder->get_widget("Prim Poly", prim_poly_widget);
  builder->get_widget("Gen Poly", gen_poly_widget);
  builder->get_widget("msg", msg_widget);
  builder->get_widget("output", output_widget);
  builder->get_widget("info", info_widget);
  
  string gfeStr = gf_widget->get_active_text();
  string primpolyStr = prim_poly_widget->get_active_id() ;
  string genpolyStr = gen_poly_widget->get_active_id();
  string msgStr = msg_widget->get_buffer()->get_text();
  stringstream ss;
  
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

  auto start = chrono::steady_clock::now();
  
  Decoder decoder = Decoder();
  Package package = decoder.decode(
      parse.getGfe(), 
      parse.getPrimpoly(), 
      parse.getGenpoly(), 
      parse.getMsg());

  auto end = chrono::steady_clock::now();
  
  int data_size = sizeof(parse.getMsg());
  double elapsed_time_us = chrono::duration_cast<chrono::microseconds>(end - start).count();

  ss << "MSG: " << data_size << " B; Elapsed Time: " << elapsed_time_us << " µs; xfer rate: "<< ( data_size * 8 * 1000000 ) / ( elapsed_time_us * 1024 ) << " kbps";
    
  info_widget->get_buffer()->set_text(ss.str());  

  if(package.getSuccess()) {
    output_widget->get_buffer()->set_text(package.getOutPolyStr());
  } else {
    error_msg_widget->set_text(package.getErrorMessage());
    error_window->show();
  }

}

void populate_combo_box(Glib::RefPtr<Gtk::Builder> builder) {

  Gtk::ComboBoxText* gf_widget;
  builder->get_widget("GF Size", gf_widget);

  ifstream infile("rsed.config");
  string lineTemp;

 while(!infile.eof()) {
    getline(infile, lineTemp);

    if(lineTemp.length() == 0) {
      continue;
    }

    if(lineTemp.find("GF:") != string::npos) {
      stringstream ss(lineTemp);
      string label;
      string gfValue;

      getline(ss, label, ':');
      getline(ss, gfValue, ':');
      gf_widget->append(gfValue);
    }
 }

 infile.close();

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

  ifstream infile("rsed.config");
  string lineTemp;
  bool selectedField = false;

  while(!infile.eof()) {
    getline(infile, lineTemp);

    if(lineTemp.length() == 0) {
      continue;
    }

    if(lineTemp.find("GF:") != string::npos) {
      stringstream ss(lineTemp);
      string label;
      string gfValue;

      getline(ss, label, ':');
      getline(ss, gfValue, ':');
      if(gf_widget->get_active_text() == gfValue) {
        selectedField = true;
      }
    }

    if(selectedField) {
      
      getline(infile, lineTemp);
      bool isActivePrimPolyId = true;
      bool isActiveGenPolyId = true;

      while (lineTemp.length() != 0) {

        stringstream ss(lineTemp);
        string label;
        string value;

        getline(ss, label, ':');
        getline(ss, value, ':');

        stringstream token(value);

        string tokenId;
        string tokenValue;

        getline(token, tokenId, '|');
        getline(token, tokenValue, '|');

        if(lineTemp.find("Prim:") != string::npos) {
            prim_poly_widget->append(tokenId,tokenValue);
            if(isActivePrimPolyId) {
              prim_poly_widget->set_active_id(tokenId);
            }
            isActivePrimPolyId = false;
        } else if(lineTemp.find("Gen:") != string::npos) {
            gen_poly_widget->append(tokenId,tokenValue);
            if(isActiveGenPolyId) {
              gen_poly_widget->set_active_id(tokenId);
            }
            isActiveGenPolyId = false;
        }
        getline(infile, lineTemp);
      }
      break;
    }
  }

  infile.close();
  
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

  populate_combo_box(builder);

  gf_widget->signal_changed().connect(sigc::bind<Glib::RefPtr<Gtk::Builder>>(sigc::ptr_fun(&change_combo_box), builder));
  
  encodeButton->signal_clicked().connect(sigc::bind<Glib::RefPtr<Gtk::Builder>>(sigc::ptr_fun(&encode_button_clicked), builder));
  decodeButton->signal_clicked().connect(sigc::bind<Glib::RefPtr<Gtk::Builder>>(sigc::ptr_fun(&decode_button_clicked), builder));
  errorButton->signal_clicked().connect(sigc::bind<Gtk::Window*>(sigc::ptr_fun(&close_window), errorWindow));

  return app->run(*window);
}
