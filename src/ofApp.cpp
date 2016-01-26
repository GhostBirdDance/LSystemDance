#include "ofApp.h"

string result = "";
//--------------------------------------------------------------
void ofApp::setup(){
    
    if( XML.load("mySettings.xml") ){
        cout << "mySettings.xml loaded!" <<endl;
        if(XML.exists("AXIOM")) {
            system.setStart(XML.getValue<string>("AXIOM"));
        }
        if(XML.exists("VARIABLES")) {
            XML.setTo("VARIABLES");
            XML.setToChild(0);
            do {
                string var = XML.getValue<string>("var");
                system.addVariable(var);
                variableDance[var] = XML.getValue<string>("move");
            } while (XML.setToSibling());
            XML.setToParent();
            XML.setToParent();

        }
        if(XML.exists("RULES")) {
            XML.setTo("RULES");
            XML.setToChild(0);
            do {
                string pre = XML.getValue<string>("pre");
                string succ = XML.getValue<string>("succ");

                system.addRule(LRule(pre, succ));
            } while (XML.setToSibling());
            XML.setToParent();
            XML.setToParent();
        }
    }

    system.printVariables();
    system.printStart();
    system.printRules();
    
    result = system.getNextLevel();
    
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_LEFT );
    gui->addTextInput("Axiom");
    gui->addTextInput("Variable");
    gui->addTextInput("Rule");
    gui->addTextInput("Variable->Move");

    gui->addButton("Generate");
    gui->onTextInputEvent(this, &ofApp::onNewTextEvent);
    gui->onButtonEvent(this, &ofApp::onButtonEvent);
    
    header.load("font-verdana.ttf", 16);
    font.load("font-verdana.ttf", 12);
}
void ofApp::onButtonEvent(ofxDatGuiButtonEvent e) {
    result = system.getNextLevel();
}

void ofApp::onNewTextEvent(ofxDatGuiTextInputEvent e)
{
    string type = e.target->getLabel();
    string val =  e.target->getText();
    cout << type << endl;
    
    if (type == "AXIOM"){
        system.setStart(val);
        result = val;
    } else if ( type == "RULE" || type == "VARIABLE->MOVE") {
        smatch m;
        regex r ("\\b(\\w+)->(\\w+)");
        
        bool error =  false;
        if (std::regex_search (val,m,r)) {
            if (m.size()%2 != 3) {
                if (type == "RULE") {
                    system.addRule(LRule(m[1], m[2]));
                } else {
                    variableDance[m[1]] = m[2];
                }
            } else {
                error = true;
                cout << "[ERROR] Too Many Matches!" << endl;
            }
        } else {
            error = true;
            cout << "[ERROR] Not valid format" << endl;
        }
        
        if (error) {
            e.target->setLabelColor(ofColor::red);
        } else {
            e.target->setLabelColor(ofColor::white);
        }
    } else if (type == "VARIABLE"){
        system.addVariable(val);
        variableDance[val] = "";
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    for(int i=0; i<components.size(); i++) components[i]->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    gui->draw();
    
    
    float x = 50;
    float y = gui->getHeight()*2;
    
    header.drawString("Variables", x, y);
    y += header.getLineHeight();
    for(int i = 0; i < system.variables.size(); i++) {
        string variable = system.variables[i];
        font.drawString(variable + " : " + variableDance[variable], x, y);
        y += font.getLineHeight();
    }
    y += font.getLineHeight();
    
    header.drawString("Rules", x, y);
    y += header.getLineHeight();
    for(int i = 0; i < system.rules.size(); i++) {
        LRule rule = system.rules[i];
        string ruleString = rule.predecessor + "->" + rule.successor;
        font.drawString(ruleString, x, y);
        y += font.getLineHeight();
    }
    y += font.getLineHeight();

    font.drawString(replaceWithMoves(), ofGetWidth()-x*2, 50);

    
    ofSetColor(ofColor::black);
    ofRectangle bounds = font.getStringBoundingBox(result, ofGetWidth()/2, ofGetHeight()/2);

    font.drawString(result, bounds.x-bounds.width/2, bounds.y-bounds.height/2);
}

string ofApp::replaceWithMoves() {
    int length = result.length();	// length of the current string
    
    string *substr = new string[length];				// split into 1-char substrings
    for(int i = 0; i < length; i++) {
        substr[i] = variableDance[result.substr(i,1)];
    }
    
    string currString;						// merge into resulting string
    for(int i = 0; i < length; i++){
        currString.append(substr[i] + "\n");
    }
    return currString;
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //no data gets saved unless you hit the s key
    if(key == 's')
    {
        XML.clear();
        XML.addChild("SYSTEM");
        XML.setTo("SYSTEM");
        
        ofXml axiom;
        axiom.addValue("AXIOM", system.start);
        
        ofXml rules;
        rules.addChild("RULES");
        rules.setTo("RULES");
        for(auto rule : system.rules) {
            rule.print();
            ofXml ruleXml;
            ruleXml.addChild("RULE");
            ruleXml.setTo("RULE");
            ruleXml.addValue("pre", rule.predecessor);
            ruleXml.addValue("succ", rule.successor);
            rules.addXml(ruleXml);
        }
        
        ofXml variables;
        variables.addChild("VARIABLES");
        variables.setTo("VARIABLES");
        for(auto variable: system.variables) {
            ofXml variableXml;
            variableXml.addChild("VARIABLE");
            variableXml.setTo("VARIABLE");
            variableXml.addValue("var", variable);
            variableXml.addValue("move", variableDance[variable]);
            variables.addXml(variableXml);
        }
        
        XML.addXml(axiom);
        XML.addXml(rules);
        XML.addXml(variables);

        XML.save("mySettings.xml");
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
