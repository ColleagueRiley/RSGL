#include "../../include/linux/rsgl.hpp" 

void RSGL::notifiy(std::string title, std::string content ,std::string image){
  std::string com = "notify-send \"" + title + "\" \"" + content + "\" ";  // the command 
  
  if (image != "") 
    com += "-i \"" + image + "\""; // add in the image if a image added
  
  popen(com.data(), "r"); // run the command

}

void RSGL::messageBox(std::string message, bool question, bool error){
  std::string com = "zenity "; // command to run

  // get the args based on the type of message
  if (question) 
    com += "--question "; 

  else if (error) 
    com += "--error "; 

  else 
    com += "--warning ";
  
  com += "--text \"" + message + "\""; // add the message
  
  popen(com.data(), "r"); // run the command
}

std::vector<std::string> RSGL::fileDialog(std::string title,bool multiple,bool save, bool directory){
  char filename[1024]; 

  // cmd to run
  std::string com="zenity --file-selection --title=\""+title+"\"";
  
  // add args based on the flags
  if (multiple) 
    com += " --multiple --separator=\" \"";
  if (directory) 
    com += " --directory";
  if (save) 
    com += " --save";

  FILE *f = popen(com.c_str(), "r"); // create a pipe for the command

  fgets(filename, 1024, f); // read f into filename
  std::string fn(filename); // convert the filename into string
  
  if (multiple){ 
    // output files or file
    std::string file;
    std::vector<std::string> output;

    for (int i=0; i < (int)fn.size(); i++){
      if (fn[i] == ' '){ output.insert(output.end(),file); file=""; } // add file to output, then clear
      else file+=fn[i]; // add to file
    }

    return {fn}; // return a single file
  }
  
  return {fn};
} 