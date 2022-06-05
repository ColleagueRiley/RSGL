#ifndef RSGL
#include "../../include/include/linux/rsgl.hpp" 
#endif

void RSGL::notifiy(std::string title, std::string content ,std::string image){
  std::string com = "notify-send \"" + title +"\" \"" + content + "\" ";  // the command 
  if (image != "")  com += "-i \"" + image + "\""; // add in the image if a image added
  popen(com.data(),"r"); // run the command
}

void RSGL::messageBox(std::string message,bool question,bool error){
  std::string com = "zenity "; // command to run
  if (question) com+="--question "; else if (error) com+= "--error"; else com+="--warning ";
  com += "--text \"" + message + "\"";
  popen(com.data(),"r");
}

std::vector<std::string> RSGL::fileDialog(std::string title,bool multiple,bool save, bool directory){
  char filename[1024];
  std::string com="zenity --file-selection --title=\""+title+"\"";
  if (multiple) com += " --multiple --separator=\" \"";
  if (directory) com += " --directory";
  if (save) com += " --save";
  FILE *f = popen(com.data(), "r");
  fgets(filename, 1024, f);
  std::string fn(filename);
  if (multiple){ 
    std::vector<std::string> output;
    std::string file;
    for (int i=0; i < (int)fn.size(); i++){
      if (fn[i] == ' '){ output.insert(output.end(),file); file="";}
      else file+=fn[i];
    }
    return output;
  }
  
  return {fn};
} 