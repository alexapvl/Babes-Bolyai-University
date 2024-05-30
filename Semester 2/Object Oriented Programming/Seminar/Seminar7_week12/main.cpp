#include "include/chatSession.h"
#include "include/chatWindow.h"
#include <QApplication>
#include <QVBoxLayout>

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  ChatSession chat;
  ChatWindow window(&chat, "User 1");
  ChatWindow window2(&chat, "User 2");
  window.show();
  window2.show();

  return app.exec();
}