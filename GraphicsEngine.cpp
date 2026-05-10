#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <iostream>
#include<fstream>
#include <cstdlib>
#include "LogicSystem.h"
#include "FileHandler.h"
#include "SecurityLog.h"
#include "GraphicsEngine.h"
#include "Validator.h"
#include "HospitalException.h"

using namespace std;
void manualTrim(char*);
void refreshDoctorList(sf::Text*, sf::Text*, sf::Text*, sf::Text*, int);
void refreshPatientList(sf::Text*, sf::Text*, sf::Text*, sf::Text*, sf::Text*, sf::Text*, int);
void refreshUnpaidBills(sf::Text*, sf::Text*, sf::Text*, sf::Text*, int);
void refreshSecurityLog(sf::Text*, int);
void refreshAppointments(sf::Text*, sf::Text*, sf::Text*, sf::Text*, sf::Text*, sf::Text*, int);
void refreshDailyReport(sf::Text&, sf::Text*, sf::Text*, int, int);

void refreshPatAppointments(sf::Text*, sf::Text*, sf::Text*, sf::Text*, sf::Text*, sf::Text*, int, int);
void refreshPatRecords(sf::Text*, sf::Text*, sf::Text*, sf::Text*, int, int);
void refreshPatBills(sf::Text*, sf::Text*, sf::Text*, sf::Text*, sf::Text*, int, int, sf::Text&);
string getTodayStr();
void refreshDoctorAppointments(sf::Text*, sf::Text*, sf::Text*, sf::Text*, int, int);
bool processDoctorAction(int, int, string, sf::Text&);
bool submitPrescription(int, int, string, string, sf::Text&);
void refreshHistory(sf::Text*, sf::Text*, sf::Text*, int, int, int, sf::Text&);
bool manualCaseInsensitiveMatch(const char*, const char*);
string loggedInUserName = "";
int loggedInUserID = 1;
string MOCKDATE = "";
string getTodayStr() {
    if (!MOCKDATE.empty()) return MOCKDATE;
    time_t t = time(nullptr);
    tm now{};
    localtime_s(&now, &t);
    char today[11];
    sprintf_s(today, sizeof(today), "%02d-%02d-%04d", now.tm_mday, now.tm_mon + 1, now.tm_year + 1900);
    return string(today);
}


int STATE_HOME = 0;
int STATE_LOGIN_SELECTION = 1;
int STATE_LOCK_STATE = 2;
int STATE_ADMIN_LOGIN = 3;
int STATE_DOCTOR_LOGIN = 4;
int STATE_PATIENT_LOGIN = 5;
int STATE_ADMIN_DASHBOARD = 6;
int STATE_DOCTOR_DASHBOARD = 7;
int STATE_PATIENT_DASHBOARD = 8;
int STATE_ADMIN_ADD_DOCTOR = 9;
int STATE_ADMIN_REMOVE_DOCTOR = 10;
int STATE_ADMIN_VIEW_PATIENTS = 11;
int STATE_ADMIN_VIEW_DOCTORS = 12;
int STATE_ADMIN_VIEW_APPOINTMENTS = 13;
int STATE_ADMIN_VIEW_BILLS = 14;
int STATE_ADMIN_DISCHARGE_PATIENT = 15;
int STATE_ADMIN_VIEW_SECURITY_LOG = 16;
int STATE_ADMIN_GENERATE_REPORT = 17;
int STATE_DOCTOR_VIEW_APPOINTMENTS = 18;
int STATE_DOCTOR_MARK_COMPLETE = 19;
int STATE_DOCTOR_MARK_NO_SHOW = 20;
int STATE_DOCTOR_WRITE_PRESCRIPTION = 21;
int STATE_DOCTOR_VIEW_HISTORY = 22;
int STATE_PATIENT_BOOK_APPOINTMENT = 23;
int STATE_PATIENT_CANCEL_APPOINTMENT = 24;
int STATE_PATIENT_VIEW_APPOINTMENTS = 25;
int STATE_PATIENT_VIEW_RECORDS = 26;
int STATE_PATIENT_VIEW_BILLS = 27;
int STATE_PATIENT_PAY_BILL = 28;
int STATE_PATIENT_TOP_UP_BALANCE = 29;

void GraphicsEngine() {
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Hospital Management System - Logs");
    window.setFramerateLimit(60);

    int currentState = STATE_HOME;

    sf::Clock lockoutTimer;
    sf::Font Font;
    sf::Font Mask;
    sf::Text errorMsg;
    sf::Text backgroundText;
    sf::Text id;
    sf::Text password;
    sf::Text loginText1;
    sf::Text loginText2;
    sf::Text loginText3;
    sf::Text adminText1;
    sf::Text adminText2;
    sf::Text adminText3;
    sf::Text adminText4;
    sf::Text adminText5;
    sf::Text adminText6;
    sf::Text adminText7;
    sf::Text adminText8;
    sf::Text adminText9;
    sf::Text adminText10;
    sf::Text patientText1;
    sf::Text patientText2;
    sf::Text patientText3;
    sf::Text patientText4;
    sf::Text patientText5;
    sf::Text patientText6;
    sf::Text patientText7;
    sf::Text patientText8;
    sf::Text doctorText1;
    sf::Text doctorText2;
    sf::Text doctorText3;
    sf::Text doctorText4;
    sf::Text doctorText5;
    sf::Text doctorText6;
    sf::Text lockedText;
    sf::Text countdownText;
    sf::Text welcomeText;
    sf::Texture txt;
    sf::Texture bg;
    sf::Texture admin;
    sf::Texture doctor;
    sf::Texture patient;
    sf::Sprite textPage;
    sf::Sprite adminPortal;
    sf::Sprite patientPortal;
    sf::Sprite doctorPortal;
    sf::Sprite BG;
    sf::Texture login;
    sf::Sprite loginPg;
    sf::Texture logo;
    sf::Sprite bgLogo;
    sf::Texture portal;
    sf::Sprite loginPortal;
    sf::Music bgAudio;
    sf::Music clickAudio;

    bgAudio.openFromFile("assets/Music/main-audio.wav");
    clickAudio.openFromFile("assets/Music/click-audio.wav");

    bgAudio.setLoop(true);
    bgAudio.play();

    logo.loadFromFile("assets/Image/logo.png");
    bgLogo.setTexture(logo);
    bgLogo.scale(0.2f, 0.2f);
    sf::FloatRect bounds = bgLogo.getLocalBounds();
    bgLogo.setColor(sf::Color::Red);
    bgLogo.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    bgLogo.setPosition(500.0f, 200.0f);

    login.loadFromFile("assets/Image/loginPage.png");
    loginPg.setTexture(login);
    loginPg.setScale(0.7f, 0.6f);
    loginPg.setPosition(0.1f, 0.0f);

    bg.loadFromFile("assets/Image/BG.png");
    BG.setTexture(bg);
    BG.setScale(0.7f, 0.6f);
    BG.setPosition(0.1f, 0.0f);

    txt.loadFromFile("assets/Image/actionPage.png");
    textPage.setTexture(txt);
    textPage.setScale(0.7f, 0.6f);
    textPage.setPosition(0.1f, 0.0f);

    portal.loadFromFile("assets/Image/loginPortal.png");
    loginPortal.setTexture(portal);
    loginPortal.setScale(0.7f, 0.6f);
    loginPortal.setPosition(0.1f, 0.0f);

    admin.loadFromFile("assets/Image/adminPage.png");
    adminPortal.setTexture(admin);
    adminPortal.setScale(0.7f, 0.6f);
    adminPortal.setPosition(0.1f, 0.0f);

    patient.loadFromFile("assets/Image/patientPage.png");
    patientPortal.setTexture(patient);
    patientPortal.setScale(0.7f, 0.6f);
    patientPortal.setPosition(0.1f, 0.0f);

    doctor.loadFromFile("assets/Image/doctorPage.png");
    doctorPortal.setTexture(doctor);
    doctorPortal.setScale(0.7f, 0.6f);
    doctorPortal.setPosition(0.1f, 0.0f);

    sf::FloatRect b;

    sf::RectangleShape adminTextBox1(sf::Vector2f(270.f, 90.f));
    adminTextBox1.setFillColor(sf::Color(255, 215, 0));
    adminTextBox1.setOutlineColor(sf::Color(139, 101, 8));
    adminTextBox1.setOutlineThickness(5);
    adminTextBox1.setScale(0.8f, 0.8f);
    b = adminTextBox1.getLocalBounds();
    adminTextBox1.setOrigin(b.width / 2.0f, b.height / 2.0f);
    adminTextBox1.setPosition(320, 180);

    sf::RectangleShape adminTextBox2(sf::Vector2f(270.f, 90.f));
    adminTextBox2.setFillColor(sf::Color(255, 215, 0));
    adminTextBox2.setOutlineColor(sf::Color(139, 101, 8));
    adminTextBox2.setOutlineThickness(5);
    adminTextBox2.setScale(0.8f, 0.8f);
    b = adminTextBox2.getLocalBounds();
    adminTextBox2.setOrigin(b.width / 2.0f, b.height / 2.0f);
    adminTextBox2.setPosition(320, 280);

    sf::RectangleShape adminTextBox3(sf::Vector2f(270.f, 90.f));
    adminTextBox3.setFillColor(sf::Color(255, 215, 0));
    adminTextBox3.setOutlineColor(sf::Color(139, 101, 8));
    adminTextBox3.setOutlineThickness(5);
    adminTextBox3.setScale(0.8f, 0.8f);
    b = adminTextBox3.getLocalBounds();
    adminTextBox3.setOrigin(b.width / 2.0f, b.height / 2.0f);
    adminTextBox3.setPosition(320, 380);

    sf::RectangleShape adminTextBox4(sf::Vector2f(270.f, 90.f));
    adminTextBox4.setFillColor(sf::Color(255, 215, 0));
    adminTextBox4.setOutlineColor(sf::Color(139, 101, 8));
    adminTextBox4.setOutlineThickness(5);
    adminTextBox4.setScale(0.8f, 0.8f);
    b = adminTextBox4.getLocalBounds();
    adminTextBox4.setOrigin(b.width / 2.0f, b.height / 2.0f);
    adminTextBox4.setPosition(550, 180);

    sf::RectangleShape adminTextBox5(sf::Vector2f(270.f, 90.f));
    adminTextBox5.setFillColor(sf::Color(255, 215, 0));
    adminTextBox5.setOutlineColor(sf::Color(139, 101, 8));
    adminTextBox5.setOutlineThickness(5);
    adminTextBox5.setScale(0.8f, 0.8f);
    b = adminTextBox5.getLocalBounds();
    adminTextBox5.setOrigin(b.width / 2.0f, b.height / 2.0f);
    adminTextBox5.setPosition(550, 280);

    sf::RectangleShape adminTextBox6(sf::Vector2f(270.f, 90.f));
    adminTextBox6.setFillColor(sf::Color(255, 215, 0));
    adminTextBox6.setOutlineColor(sf::Color(139, 101, 8));
    adminTextBox6.setOutlineThickness(5);
    adminTextBox6.setScale(0.8f,0.8f);
    b = adminTextBox6.getLocalBounds();
    adminTextBox6.setOrigin(b.width / 2.0f, b.height / 2.0f);
    adminTextBox6.setPosition(550, 380);

    sf::RectangleShape adminTextBox7(sf::Vector2f(270.f, 90.f));
    adminTextBox7.setFillColor(sf::Color(255, 215, 0));
    adminTextBox7.setOutlineColor(sf::Color(139, 101, 8));
    adminTextBox7.setOutlineThickness(5);
    adminTextBox7.setScale(0.8f, 0.8f);
    b = adminTextBox7.getLocalBounds();
    adminTextBox7.setOrigin(b.width / 2.0f, b.height / 2.0f);
    adminTextBox7.setPosition(780, 180);

    sf::RectangleShape adminTextBox8(sf::Vector2f(270.f, 90.f));
    adminTextBox8.setFillColor(sf::Color(255, 215, 0));
    adminTextBox8.setOutlineColor(sf::Color(139, 101, 8));
    adminTextBox8.setOutlineThickness(5);
    adminTextBox8.setScale(0.8f, 0.8f);
    b = adminTextBox8.getLocalBounds();
    adminTextBox8.setOrigin(b.width / 2.0f, b.height / 2.0f);
    adminTextBox8.setPosition(780, 280);

    sf::RectangleShape adminTextBox9(sf::Vector2f(270.f, 90.f));
    adminTextBox9.setFillColor(sf::Color(255, 215, 0));
    adminTextBox9.setOutlineColor(sf::Color(139, 101, 8));
    adminTextBox9.setOutlineThickness(5);
    adminTextBox9.setScale(0.8f, 0.8f);
    b = adminTextBox9.getLocalBounds();
    adminTextBox9.setOrigin(b.width / 2.0f, b.height / 2.0f);
    adminTextBox9.setPosition(780, 380);

    sf::RectangleShape adminTextBox10(sf::Vector2f(270.f, 90.f));
    adminTextBox10.setFillColor(sf::Color(255, 215, 0));
    adminTextBox10.setOutlineColor(sf::Color(139, 101, 8));
    adminTextBox10.setOutlineThickness(5);
    adminTextBox10.setScale(0.8f, 0.8f);
    b = adminTextBox10.getLocalBounds();
    adminTextBox10.setOrigin(b.width / 2.0f, b.height / 2.0f);
    adminTextBox10.setPosition(550, 470);

    sf::RectangleShape portalTextBox1(sf::Vector2f(270.f, 90.f));
    portalTextBox1.setFillColor(sf::Color(150, 150, 150));
    portalTextBox1.setOutlineColor(sf::Color::Black);
    portalTextBox1.setOutlineThickness(5);
    portalTextBox1.setScale(1.5f, 0.8f);
    b = portalTextBox1.getLocalBounds();
    portalTextBox1.setOrigin(b.width / 2.0f, b.height / 2.0f);
    portalTextBox1.setPosition(620, 230);

    sf::RectangleShape portalTextBox2(sf::Vector2f(270.f, 90.f));
    portalTextBox2.setFillColor(sf::Color(150, 150, 150));
    portalTextBox2.setOutlineColor(sf::Color::Black);
    portalTextBox2.setOutlineThickness(5);
    portalTextBox2.setScale(1.5f, 0.8f);
    b = portalTextBox2.getLocalBounds();
    portalTextBox2.setOrigin(b.width / 2.0f, b.height / 2.0f);
    portalTextBox2.setPosition(620, 330);

    sf::RectangleShape loginTextBox1(sf::Vector2f(270.f, 90.f));
    loginTextBox1.setFillColor(sf::Color(60, 60, 255));
    loginTextBox1.setOutlineColor(sf::Color(20, 20, 150));
    loginTextBox1.setOutlineThickness(5);
    loginTextBox1.setScale(2.0f, 0.8f);
    b = loginTextBox1.getLocalBounds();
    loginTextBox1.setOrigin(b.width / 2.0f, b.height / 2.0f);
    loginTextBox1.setPosition(540, 200);

    sf::RectangleShape loginTextBox2(sf::Vector2f(270.f, 90.f));
    loginTextBox2.setFillColor(sf::Color(60, 60, 255));
    loginTextBox2.setOutlineColor(sf::Color(20, 20, 150));
    loginTextBox2.setOutlineThickness(5);
    loginTextBox2.setScale(2.0f, 0.8f);
    b = loginTextBox2.getLocalBounds();
    loginTextBox2.setOrigin(b.width / 2.0f, b.height / 2.0f);
    loginTextBox2.setPosition(540, 300);

    sf::RectangleShape loginTextBox3(sf::Vector2f(270.f, 90.f));
    loginTextBox3.setFillColor(sf::Color(60, 60, 255));
    loginTextBox3.setOutlineColor(sf::Color(20, 20, 150));
    loginTextBox3.setOutlineThickness(5);
    loginTextBox3.setScale(2.0f, 0.8f);
    b = loginTextBox3.getLocalBounds();
    loginTextBox3.setOrigin(b.width / 2.0f, b.height / 2.0f);
    loginTextBox3.setPosition(540, 400);

    sf::RectangleShape bgTextBox(sf::Vector2f(270.f, 90.f));
    bgTextBox.setFillColor(sf::Color(255, 215, 0));
    bgTextBox.setOutlineColor(sf::Color::Black);
    bgTextBox.setOutlineThickness(5);
    bgTextBox.setScale(2.4f, 1.0f);
    b = bgTextBox.getLocalBounds();
    bgTextBox.setOrigin(b.width / 2.0f, b.height / 2.0f);
    bgTextBox.setPosition(510, 400);

    sf::RectangleShape patientTextBox1(sf::Vector2f(270.f, 90.f));
    patientTextBox1.setFillColor(sf::Color(255, 215, 0));
    patientTextBox1.setOutlineColor(sf::Color(139, 101, 8));
    patientTextBox1.setOutlineThickness(5);
    patientTextBox1.setScale(0.8f, 0.8f);
    b = patientTextBox1.getLocalBounds();
    patientTextBox1.setOrigin(b.width / 2.0f, b.height / 2.0f);
    patientTextBox1.setPosition(320, 180);

    sf::RectangleShape patientTextBox2(sf::Vector2f(270.f, 90.f));
    patientTextBox2.setFillColor(sf::Color(255, 215, 0));
    patientTextBox2.setOutlineColor(sf::Color(139, 101, 8));
    patientTextBox2.setOutlineThickness(5);
    patientTextBox2.setScale(0.8f, 0.8f);
    b = patientTextBox2.getLocalBounds();
    patientTextBox2.setOrigin(b.width / 2.0f, b.height / 2.0f);
    patientTextBox2.setPosition(320, 280);

    sf::RectangleShape patientTextBox3(sf::Vector2f(270.f, 90.f));
    patientTextBox3.setFillColor(sf::Color(255, 215, 0));
    patientTextBox3.setOutlineColor(sf::Color(139, 101, 8));
    patientTextBox3.setOutlineThickness(5);
    patientTextBox3.setScale(0.8f, 0.8f);
    b = patientTextBox3.getLocalBounds();
    patientTextBox3.setOrigin(b.width / 2.0f, b.height / 2.0f);
    patientTextBox3.setPosition(320, 380);

    sf::RectangleShape patientTextBox4(sf::Vector2f(270.f, 90.f));
    patientTextBox4.setFillColor(sf::Color(255, 215, 0));
    patientTextBox4.setOutlineColor(sf::Color(139, 101, 8));
    patientTextBox4.setOutlineThickness(5);
    patientTextBox4.setScale(0.8f, 0.8f);
    b = patientTextBox4.getLocalBounds();
    patientTextBox4.setOrigin(b.width / 2.0f, b.height / 2.0f);
    patientTextBox4.setPosition(550, 180);

    sf::RectangleShape patientTextBox5(sf::Vector2f(270.f, 90.f));
    patientTextBox5.setFillColor(sf::Color(255, 215, 0));
    patientTextBox5.setOutlineColor(sf::Color(139, 101, 8));
    patientTextBox5.setOutlineThickness(5);
    patientTextBox5.setScale(0.8f, 0.8f);
    b = patientTextBox5.getLocalBounds();
    patientTextBox5.setOrigin(b.width / 2.0f, b.height / 2.0f);
    patientTextBox5.setPosition(550, 280);

    sf::RectangleShape patientTextBox6(sf::Vector2f(270.f, 90.f));
    patientTextBox6.setFillColor(sf::Color(255, 215, 0));
    patientTextBox6.setOutlineColor(sf::Color(139, 101, 8));
    patientTextBox6.setOutlineThickness(5);
    patientTextBox6.setScale(0.8f, 0.8f);
    b = patientTextBox6.getLocalBounds();
    patientTextBox6.setOrigin(b.width / 2.0f, b.height / 2.0f);
    patientTextBox6.setPosition(550, 380);

    sf::RectangleShape patientTextBox7(sf::Vector2f(270.f, 90.f));
    patientTextBox7.setFillColor(sf::Color(255, 215, 0));
    patientTextBox7.setOutlineColor(sf::Color(139, 101, 8));
    patientTextBox7.setOutlineThickness(5);
    patientTextBox7.setScale(0.8f, 0.8f);
    b = patientTextBox7.getLocalBounds();
    patientTextBox7.setOrigin(b.width / 2.0f, b.height / 2.0f);
    patientTextBox7.setPosition(780, 180);

    sf::RectangleShape patientTextBox8(sf::Vector2f(270.f, 90.f));
    patientTextBox8.setFillColor(sf::Color(255, 215, 0));
    patientTextBox8.setOutlineColor(sf::Color(139, 101, 8));
    patientTextBox8.setOutlineThickness(5);
    patientTextBox8.setScale(0.8f, 0.8f);
    b = patientTextBox8.getLocalBounds();
    patientTextBox8.setOrigin(b.width / 2.0f, b.height / 2.0f);
    patientTextBox8.setPosition(780, 280);

    sf::RectangleShape doctorTextBox1(sf::Vector2f(270.f, 90.f));
    doctorTextBox1.setFillColor(sf::Color(255, 215, 0));
    doctorTextBox1.setOutlineColor(sf::Color(139, 101, 8));
    doctorTextBox1.setOutlineThickness(5);
    doctorTextBox1.setScale(0.8f, 0.8f);
    b = doctorTextBox1.getLocalBounds();
    doctorTextBox1.setOrigin(b.width / 2.0f, b.height / 2.0f);
    doctorTextBox1.setPosition(370, 180);

    sf::RectangleShape doctorTextBox2(sf::Vector2f(270.f, 90.f));
    doctorTextBox2.setFillColor(sf::Color(255, 215, 0));
    doctorTextBox2.setOutlineColor(sf::Color(139, 101, 8));
    doctorTextBox2.setOutlineThickness(5);
    doctorTextBox2.setScale(0.8f, 0.8f);
    b = doctorTextBox2.getLocalBounds();
    doctorTextBox2.setOrigin(b.width / 2.0f, b.height / 2.0f);
    doctorTextBox2.setPosition(370, 280);

    sf::RectangleShape doctorTextBox3(sf::Vector2f(270.f, 90.f));
    doctorTextBox3.setFillColor(sf::Color(255, 215, 0));
    doctorTextBox3.setOutlineColor(sf::Color(139, 101, 8));
    doctorTextBox3.setOutlineThickness(5);
    doctorTextBox3.setScale(0.8f, 0.8f);
    b = doctorTextBox3.getLocalBounds();
    doctorTextBox3.setOrigin(b.width / 2.0f, b.height / 2.0f);
    doctorTextBox3.setPosition(370, 380);

    sf::RectangleShape doctorTextBox4(sf::Vector2f(270.f, 90.f));
    doctorTextBox4.setFillColor(sf::Color(255, 215, 0));
    doctorTextBox4.setOutlineColor(sf::Color(139, 101, 8));
    doctorTextBox4.setOutlineThickness(5);
    doctorTextBox4.setScale(0.8f, 0.8f);
    b = doctorTextBox4.getLocalBounds();
    doctorTextBox4.setOrigin(b.width / 2.0f, b.height / 2.0f);
    doctorTextBox4.setPosition(650, 180);

    sf::RectangleShape doctorTextBox5(sf::Vector2f(270.f, 90.f));
    doctorTextBox5.setFillColor(sf::Color(255, 215, 0));
    doctorTextBox5.setOutlineColor(sf::Color(139, 101, 8));
    doctorTextBox5.setOutlineThickness(5);
    doctorTextBox5.setScale(0.8f, 0.8f);
    b = doctorTextBox5.getLocalBounds();
    doctorTextBox5.setOrigin(b.width / 2.0f, b.height / 2.0f);
    doctorTextBox5.setPosition(650, 280);

    sf::RectangleShape doctorTextBox6(sf::Vector2f(270.f, 90.f));
    doctorTextBox6.setFillColor(sf::Color(255, 215, 0));
    doctorTextBox6.setOutlineColor(sf::Color(139, 101, 8));
    doctorTextBox6.setOutlineThickness(5);
    doctorTextBox6.setScale(0.8f, 0.8f);
    b = doctorTextBox6.getLocalBounds();
    doctorTextBox6.setOrigin(b.width / 2.0f, b.height / 2.0f);
    doctorTextBox6.setPosition(650, 380);


    Font.loadFromFile("assets/Text/game.TTF");
    Mask.loadFromFile("assets/Text/arial.ttf");
    loginText1.setFont(Font);
    loginText1.setCharacterSize(40);
    loginText1.setString("Admin");
    bounds = loginText1.getLocalBounds();
    loginText1.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    loginText1.setPosition(520.0f, 180.0f);

    sf::RectangleShape portalBackBtn(sf::Vector2f(120.f, 45.f));
    portalBackBtn.setFillColor(sf::Color(200, 50, 50));
    portalBackBtn.setOutlineColor(sf::Color(150, 0, 0));
    portalBackBtn.setOutlineThickness(3);
    portalBackBtn.setPosition(20.f, 20.f);

    sf::Text portalBackTxt;
    portalBackTxt.setFont(Font);
    portalBackTxt.setCharacterSize(22);
    portalBackTxt.setString("< Back");
    portalBackTxt.setFillColor(sf::Color::White);
    portalBackTxt.setPosition(28.f, 28.f);

    loginText2.setFont(Font);
    loginText2.setCharacterSize(43);
    loginText2.setString("Doctor");
    bounds = loginText2.getLocalBounds();
    loginText2.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    loginText2.setPosition(530.0f, 280.0f);

    loginText3.setFont(Font);
    loginText3.setCharacterSize(40);
    loginText3.setString("Patient");
    bounds = loginText3.getLocalBounds();
    loginText3.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    loginText3.setPosition(530.0f, 380.0f);

    doctorText1.setFont(Font);
    doctorText1.setCharacterSize(20);
    doctorText1.setString(" View Today's\nAppointments");
    bounds = doctorText1.getLocalBounds();
    doctorText1.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    doctorText1.setPosition(365.0f, 170.0f);

    doctorText2.setFont(Font);
    doctorText2.setCharacterSize(20);
    doctorText2.setString("    Mark\n Appointment\n  Complete");
    bounds = doctorText2.getLocalBounds();
    doctorText2.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    doctorText2.setPosition(365.0f, 270.0f);

    doctorText3.setFont(Font);
    doctorText3.setCharacterSize(20);
    doctorText3.setString("   Mark\nAppointment\n  No-Show");
    bounds = doctorText3.getLocalBounds();
    doctorText3.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    doctorText3.setPosition(365.0f, 370.0f);

    doctorText4.setFont(Font);
    doctorText4.setCharacterSize(20);
    doctorText4.setString("   Write\nPrescription");
    bounds = doctorText4.getLocalBounds();
    doctorText4.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    doctorText4.setPosition(645.0f, 170.0f);

    doctorText5.setFont(Font);
    doctorText5.setCharacterSize(20);
    doctorText5.setString(" View Patient\n   Medical\n   History");
    bounds = doctorText5.getLocalBounds();
    doctorText5.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    doctorText5.setPosition(645.0f, 270.0f);

    doctorText6.setFont(Font);
    doctorText6.setCharacterSize(20);
    doctorText6.setString("Logout");
    bounds = doctorText6.getLocalBounds();
    doctorText6.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    doctorText6.setPosition(645.0f, 370.0f);

    adminText1.setFont(Font);
    adminText1.setCharacterSize(20);
    adminText1.setString("Add Doctor");
    bounds = adminText1.getLocalBounds();
    adminText1.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    adminText1.setPosition(315.0f, 170.0f);

    adminText2.setFont(Font);
    adminText2.setCharacterSize(20);
    adminText2.setString("Remove Doctor");
    bounds = adminText2.getLocalBounds();
    adminText2.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    adminText2.setPosition(315.0f, 270.0f);

    adminText3.setFont(Font);
    adminText3.setCharacterSize(20);
    adminText3.setString("View All\nPatients");
    bounds = adminText3.getLocalBounds();
    adminText3.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    adminText3.setPosition(315.0f, 370.0f);

    adminText4.setFont(Font);
    adminText4.setCharacterSize(20);
    adminText4.setString("View All\nDoctors");
    bounds = adminText4.getLocalBounds();
    adminText4.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    adminText4.setPosition(545.0f, 170.0f);

    adminText5.setFont(Font);
    adminText5.setCharacterSize(20);
    adminText5.setString("  View All\nAppointments");
    bounds = adminText5.getLocalBounds();
    adminText5.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    adminText5.setPosition(545.0f, 270.0f);

    adminText6.setFont(Font);
    adminText6.setCharacterSize(20);
    adminText6.setString("View Unpaid\n   Bills");
    bounds = adminText6.getLocalBounds();
    adminText6.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    adminText6.setPosition(545.0f, 370.0f);

    adminText7.setFont(Font);
    adminText7.setCharacterSize(20);
    adminText7.setString("Discharge\nPatient");
    bounds = adminText7.getLocalBounds();
    adminText7.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    adminText7.setPosition(775.0f, 170.0f);

    adminText8.setFont(Font);
    adminText8.setCharacterSize(20);
    adminText8.setString("View Security\n     Log");
    bounds = adminText8.getLocalBounds();
    adminText8.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    adminText8.setPosition(775.0f, 270.0f);

    adminText9.setFont(Font);
    adminText9.setCharacterSize(20);
    adminText9.setString("   Generate \nDaily Report");
    bounds = adminText9.getLocalBounds();
    adminText9.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    adminText9.setPosition(775.0f, 370.0f);

    adminText10.setFont(Font);
    adminText10.setCharacterSize(22);
    adminText10.setString("Logout");
    bounds = adminText10.getLocalBounds();
    adminText10.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    adminText10.setPosition(545.0f, 460.0f);

    welcomeText.setFont(Font);
    welcomeText.setCharacterSize(30);
    welcomeText.setFillColor(sf::Color::Black);

    patientText1.setFont(Font);
    patientText1.setCharacterSize(20);
    patientText1.setString("    Book\nAppointment");
    bounds = patientText1.getLocalBounds();
    patientText1.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    patientText1.setPosition(315.0f, 170.0f);

    patientText2.setFont(Font);
    patientText2.setCharacterSize(20);
    patientText2.setString("   Cancel\nAppointment");
    bounds = patientText2.getLocalBounds();
    patientText2.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    patientText2.setPosition(315.0f, 270.0f);

    patientText3.setFont(Font);
    patientText3.setCharacterSize(20);
    patientText3.setString("    View\n     My\nAppointments");
    bounds = patientText3.getLocalBounds();
    patientText3.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    patientText3.setPosition(315.0f, 370.0f);

    patientText4.setFont(Font);
    patientText4.setCharacterSize(20);
    patientText4.setString(" View My \n Medical\n Records");
    bounds = patientText4.getLocalBounds();
    patientText4.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    patientText4.setPosition(545.0f, 170.0f);

    patientText5.setFont(Font);
    patientText5.setCharacterSize(20);
    patientText5.setString("  View\nMy Bills");
    bounds = patientText5.getLocalBounds();
    patientText5.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    patientText5.setPosition(545.0f, 270.0f);

    patientText6.setFont(Font);
    patientText6.setCharacterSize(20);
    patientText6.setString("Pay Bill");
    bounds = patientText6.getLocalBounds();
    patientText6.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    patientText6.setPosition(545.0f, 370.0f);

    patientText7.setFont(Font);
    patientText7.setCharacterSize(20);
    patientText7.setString("Top Up\nBalance");
    bounds = patientText7.getLocalBounds();
    patientText7.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    patientText7.setPosition(775.0f, 170.0f);

    patientText8.setFont(Font);
    patientText8.setCharacterSize(20);
    patientText8.setString("Logout");
    bounds = patientText8.getLocalBounds();
    patientText8.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    patientText8.setPosition(775.0f, 270.0f);

    lockedText.setFont(Font);
    lockedText.setCharacterSize(60);
    lockedText.setFillColor(sf::Color::Red);
    lockedText.setString("SYSTEM LOCKED");
    sf::FloatRect lockBounds = lockedText.getLocalBounds();
    lockedText.setOrigin(lockBounds.width / 2.0f, lockBounds.height / 2.0f);
    lockedText.setPosition(500.0f, 250.0f);

    countdownText.setFont(Font);
    countdownText.setCharacterSize(35);
    countdownText.setFillColor(sf::Color::White);
    countdownText.setString("Please wait 5 seconds...");
    sf::FloatRect countBounds = countdownText.getLocalBounds();
    countdownText.setOrigin(countBounds.width / 2.0f, countBounds.height / 2.0f);
    countdownText.setPosition(500.0f, 350.0f);

    id.setFont(Font);
    id.setCharacterSize(34);
    id.setString("Enter ID: ");
    bounds = id.getLocalBounds();
    id.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    id.setPosition(300.0f, 220.0f);

    password.setFont(Font);
    password.setCharacterSize(34);
    password.setString("Enter\nPassword: ");
    password.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    password.setPosition(300.0f, 300.0f);

    backgroundText.setFont(Font);
    backgroundText.setCharacterSize(44);
    backgroundText.setString("MEDICORE HOSPITAL");
    bounds = backgroundText.getLocalBounds();
    backgroundText.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    backgroundText.setPosition(500.0f, 375.0f);

    bool isIdActive = false;
    bool isPassActive = false;
    char idBuffer[32] = "";
    int idCharCount = 0;

    char passBuffer[32] = "";
    int passCharCount = 0;

    sf::Text displayLine1;
    displayLine1.setFont(Mask);
    displayLine1.setCharacterSize(28);
    displayLine1.setPosition(440.0f, 310.0f);

    sf::Text displayLine2;
    displayLine2.setFont(Font);
    displayLine2.setCharacterSize(25);
    displayLine2.setPosition(440.0f, 210.0f);

    bool showError = false;
    errorMsg.setFont(Font);
    errorMsg.setCharacterSize(35);
    errorMsg.setFillColor(sf::Color::Red);
    errorMsg.setString("X Wrong ID or Password!");
    sf::FloatRect errorBounds = errorMsg.getLocalBounds();
    errorMsg.setOrigin(errorBounds.width / 2.0f, errorBounds.height / 2.0f);
    errorMsg.setPosition(500.0f, 500.0f);

    sf::Text saveTxt;
    sf::Text backTxt;

    bool activeDoc[6] = { false, false, false, false, false, false };
    string docInput[6] = { "", "", "", "", "", "" };

    sf::RectangleShape docBoxes[6];

    sf::Text doctorRows[10];
    sf::Text docLabels[6];
    sf::Text docDisplay[6];
    sf::Text addDocErrorMsg;
    sf::Text removeDocDisplay;
    sf::Text removeDocLabel;
    sf::Text removeDocStatusMsg;
    sf::Text confirmRemoveTxt;

    removeDocStatusMsg.setFont(Font);
    removeDocStatusMsg.setCharacterSize(22);
    removeDocStatusMsg.setPosition(250.f, 250.f);

    string labelStrings[6] = { "Doctor ID:", "Name:", "Specialty:", "Contact:", "Password:", "Fee:" };

    for (int i = 0; i < 6; i++) {
        docBoxes[i].setSize(sf::Vector2f(350.f, 40.f));
        docBoxes[i].setFillColor(sf::Color(150, 150, 150));
        docBoxes[i].setOutlineColor(sf::Color::Black);
        docBoxes[i].setOutlineThickness(3);
        docBoxes[i].setPosition(450.f, 100.f + (i * 70.f));

        docLabels[i].setFont(Font);
        docLabels[i].setCharacterSize(25);
        docLabels[i].setString(labelStrings[i]);
        docLabels[i].setFillColor(sf::Color(204, 170, 0));
        docLabels[i].setPosition(250.f, 105.f + (i * 70.f));

        docDisplay[i].setFont(Font);
        docDisplay[i].setCharacterSize(22);
        docDisplay[i].setFillColor(sf::Color::Black);
        docDisplay[i].setPosition(460.f, 105.f + (i * 70.f));
    }

    sf::RectangleShape backBtn(sf::Vector2f(100.f, 40.f));
    backBtn.setFillColor(sf::Color::Red);
    backBtn.setPosition(20.f, 20.f);
    backTxt.setFont(Font);
    backTxt.setCharacterSize(20);
    backTxt.setString("Back");
    backTxt.setPosition(40.f, 25.f);

    sf::RectangleShape saveBtn(sf::Vector2f(200.f, 60.f));
    saveBtn.setFillColor(sf::Color(50, 200, 50));
    saveBtn.setPosition(525.f, 510.f);
    saveTxt.setFont(Font);
    saveTxt.setCharacterSize(35);
    saveTxt.setString("SAVE");
    saveTxt.setPosition(575.f, 520.f);

    sf::Text navBackTxt, navNextTxt, navPrevTxt;

    navBackTxt.setFont(Font); navBackTxt.setCharacterSize(20);
    navBackTxt.setString("Back"); navBackTxt.setPosition(40.f, 25.f);

    navNextTxt.setFont(Font); navNextTxt.setCharacterSize(20);
    navNextTxt.setString("Next >"); navNextTxt.setPosition(725.f, 525.f);

    navPrevTxt.setFont(Font); navPrevTxt.setCharacterSize(20);
    navPrevTxt.setString("< Prev"); navPrevTxt.setPosition(205.f, 525.f);

    bool showAddDocError = false;
    addDocErrorMsg.setFont(Font);
    addDocErrorMsg.setCharacterSize(25);
    addDocErrorMsg.setFillColor(sf::Color::Yellow);
    addDocErrorMsg.setPosition(230.f, 45.f);

    bool activeRemoveDoc = false;
    string removeDocInput = "";

    sf::RectangleShape removeDocBox(sf::Vector2f(350.f, 50.f));
    removeDocBox.setFillColor(sf::Color(150, 150, 150));
    removeDocBox.setOutlineColor(sf::Color::Black);
    removeDocBox.setOutlineThickness(3);
    removeDocBox.setPosition(450.f, 300.f);

    removeDocLabel.setFont(Font);
    removeDocLabel.setCharacterSize(30);
    removeDocLabel.setString("Doctor ID:");
    removeDocLabel.setFillColor(sf::Color(204, 170, 0));
    removeDocLabel.setPosition(210.f, 305.f);

    removeDocDisplay.setFont(Font);
    removeDocDisplay.setCharacterSize(25);
    removeDocDisplay.setFillColor(sf::Color::Black);
    removeDocDisplay.setPosition(460.f, 310.f);

    sf::RectangleShape confirmRemoveBtn(sf::Vector2f(250.f, 60.f));
    confirmRemoveBtn.setFillColor(sf::Color(200, 50, 50));
    confirmRemoveBtn.setPosition(500.f, 400.f);

    confirmRemoveTxt.setFont(Font);
    confirmRemoveTxt.setCharacterSize(30);
    confirmRemoveTxt.setFillColor(sf::Color::White);
    confirmRemoveTxt.setString("REMOVE");
    confirmRemoveTxt.setPosition(545.f, 410.f);

    int currentDocPage = 0;

    sf::Text viewDocHeader;
    viewDocHeader.setFont(Font);
    viewDocHeader.setCharacterSize(30);
    viewDocHeader.setFillColor(sf::Color(204, 170, 0));
    viewDocHeader.setString("ID  Name    Specialty  Fee");
    viewDocHeader.setPosition(200.f, 110.f);

    sf::Text nextTxt;
    nextTxt.setFont(Font);
    nextTxt.setCharacterSize(20);
    nextTxt.setFillColor(sf::Color::Black);
    nextTxt.setString("Next >");

    nextTxt.setPosition(725.f, 525.f);

    sf::Text colID[10], colName[10], colSpec[10], colFee[10];
    for (int i = 0; i < 10; i++) {
        float yPos = 160.f + (i * 35.f);

        colID[i].setFont(Font);   colID[i].setCharacterSize(20); colID[i].setPosition(200.f, yPos);
        colName[i].setFont(Font); colName[i].setCharacterSize(20); colName[i].setPosition(270.f, yPos);
        colSpec[i].setFont(Font); colSpec[i].setCharacterSize(20); colSpec[i].setPosition(530.f, yPos);
        colFee[i].setFont(Font);  colFee[i].setCharacterSize(20); colFee[i].setPosition(780.f, yPos);

        colID[i].setFillColor(sf::Color::Black);
        colName[i].setFillColor(sf::Color::Black);
        colSpec[i].setFillColor(sf::Color::Black);
        colFee[i].setFillColor(sf::Color::Black);
    }
    sf::RectangleShape nextBtn(sf::Vector2f(80.f, 35.f));
    nextBtn.setFillColor(sf::Color::Green);
    nextBtn.setPosition(720.f, 520.f);

    sf::RectangleShape prevBtn(sf::Vector2f(100.f, 35.f));
    prevBtn.setFillColor(sf::Color(150, 150, 150));
    prevBtn.setPosition(200.f, 520.f);

    int currentPatPage = 0;
    sf::Text patHeader;
    patHeader.setFont(Font);
    patHeader.setCharacterSize(22); 
    patHeader.setFillColor(sf::Color(204, 170, 0));
    patHeader.setString("ID   Name   Age/G  Contact  Balance Bills");
    patHeader.setPosition(150.f, 110.f);

    sf::Text pCol1[10], pCol2[10], pCol3[10], pCol4[10], pCol5[10], pCol6[10];

    for (int i = 0; i < 10; i++) {
        float yPos = 160.f + (i * 35.f);
        pCol1[i].setFont(Font); pCol1[i].setCharacterSize(18); pCol1[i].setPosition(150.f, yPos);
        pCol2[i].setFont(Font); pCol2[i].setCharacterSize(18); pCol2[i].setPosition(200.f, yPos);
        pCol3[i].setFont(Font); pCol3[i].setCharacterSize(18); pCol3[i].setPosition(400.f, yPos);
        pCol4[i].setFont(Font); pCol4[i].setCharacterSize(18); pCol4[i].setPosition(480.f, yPos);
        pCol5[i].setFont(Font); pCol5[i].setCharacterSize(18); pCol5[i].setPosition(700.f, yPos);
        pCol6[i].setFont(Font); pCol6[i].setCharacterSize(18); pCol6[i].setPosition(860.f, yPos);

        pCol1[i].setFillColor(sf::Color::Black); pCol2[i].setFillColor(sf::Color::Black);
        pCol3[i].setFillColor(sf::Color::Black); pCol4[i].setFillColor(sf::Color::Black);
        pCol5[i].setFillColor(sf::Color::Black); pCol6[i].setFillColor(sf::Color::Black);
    }

    int currentLogPage = 0;

    sf::Text logHeader;
    logHeader.setFont(Font);
    logHeader.setCharacterSize(25);
    logHeader.setFillColor(sf::Color(204, 170, 0));
    logHeader.setString("Security Log Events:");
    logHeader.setPosition(350.f, 110.f);

    sf::Text logLines[12];
    for (int i = 0; i < 12; i++) {
        logLines[i].setFont(Font);
        logLines[i].setCharacterSize(18);
        logLines[i].setFillColor(sf::Color::Black);
        logLines[i].setPosition(250.f, 160.f + (i * 30.f));
    }

    int currentApptPage = 0;
    sf::Text apptHeader;
    apptHeader.setFont(Font);
    apptHeader.setCharacterSize(17);
    apptHeader.setFillColor(sf::Color(204, 170, 0));
    apptHeader.setString("ID  Patient Name  Doctor Name   Date      Status   Time");
    apptHeader.setPosition(150.f, 120.f);

    sf::Text aCol1[10], aCol2[10], aCol3[10], aCol4[10], aCol5[10], aCol6[10];
    for (int i = 0; i < 10; i++) {
        float yPos = 160.f + (i * 35.f);
        aCol1[i].setFont(Font); aCol1[i].setCharacterSize(14); aCol1[i].setPosition(150.f, yPos);
        aCol2[i].setFont(Font); aCol2[i].setCharacterSize(14); aCol2[i].setPosition(210.f, yPos);
        aCol3[i].setFont(Font); aCol3[i].setCharacterSize(14); aCol3[i].setPosition(390.f, yPos);
        aCol4[i].setFont(Font); aCol4[i].setCharacterSize(14); aCol4[i].setPosition(580.f, yPos);
        aCol5[i].setFont(Font); aCol5[i].setCharacterSize(14); aCol5[i].setPosition(870.f, yPos);
        aCol6[i].setFont(Font); aCol6[i].setCharacterSize(14); aCol6[i].setPosition(730.f, yPos);

        aCol1[i].setFillColor(sf::Color::Black); aCol2[i].setFillColor(sf::Color::Black);
        aCol3[i].setFillColor(sf::Color::Black); aCol4[i].setFillColor(sf::Color::Black);
        aCol5[i].setFillColor(sf::Color::Black); aCol6[i].setFillColor(sf::Color::Black);
    }

    int currentBillPage = 0;
    sf::Text billsHeader;
    billsHeader.setFont(Font);
    billsHeader.setCharacterSize(22);
    billsHeader.setFillColor(sf::Color(204, 170, 0));
    billsHeader.setString("Bill ID   Patient Name  Amount  Date");
    billsHeader.setPosition(180.f, 118.f);

    sf::Text bCol1[10], bCol2[10], bCol3[10], bCol4[10];
    for (int i = 0; i < 10; i++) {
        float yPos = 160.f + (i * 35.f);
        bCol1[i].setFont(Font); bCol1[i].setCharacterSize(18); bCol1[i].setPosition(210.f, yPos);
        bCol2[i].setFont(Font); bCol2[i].setCharacterSize(18); bCol2[i].setPosition(350.f, yPos);
        bCol3[i].setFont(Font); bCol3[i].setCharacterSize(18); bCol3[i].setPosition(620.f, yPos);
        bCol4[i].setFont(Font); bCol4[i].setCharacterSize(18); bCol4[i].setPosition(740.f, yPos);

        bCol1[i].setFillColor(sf::Color::Black); bCol2[i].setFillColor(sf::Color::Black);
        bCol3[i].setFillColor(sf::Color::Black); bCol4[i].setFillColor(sf::Color::Black);
    }

    bool activeDischargeInput = false;
    string dischargeInput = "";
    sf::Text dischargeDisplay;
    sf::Text dischargeLabel;
    sf::Text dischargeStatusMsg;
    sf::Text confirmDischargeTxt;

    dischargeLabel.setFont(Font);
    dischargeLabel.setCharacterSize(25);
    dischargeLabel.setFillColor(sf::Color(204, 170, 0));
    dischargeLabel.setString("Enter Patient ID\nto Discharge:");
    dischargeLabel.setPosition(210.f, 220.f);
    
    sf::RectangleShape dischargeBox(sf::Vector2f(320.f, 50.f));
    dischargeBox.setFillColor(sf::Color(150, 150, 150));
    dischargeBox.setOutlineThickness(3);
    dischargeBox.setOutlineColor(sf::Color::Black);
    dischargeBox.setPosition(550.f, 215.f);

    dischargeDisplay.setFont(Font);
    dischargeDisplay.setCharacterSize(25);
    dischargeDisplay.setFillColor(sf::Color::Black);
    dischargeDisplay.setPosition(560.f, 225.f);

    dischargeStatusMsg.setFont(Font);
    dischargeStatusMsg.setCharacterSize(22);
    dischargeStatusMsg.setPosition(250.f, 420.f);

    sf::RectangleShape confirmDischargeBtn(sf::Vector2f(270.f, 60.f));
    confirmDischargeBtn.setFillColor(sf::Color(200, 50, 50));
    confirmDischargeBtn.setPosition(370.f, 320.f);

    confirmDischargeTxt.setFont(Font);
    confirmDischargeTxt.setCharacterSize(25);
    confirmDischargeTxt.setString("DISCHARGE");
    confirmDischargeTxt.setFillColor(sf::Color::White);
    sf::FloatRect textRect = confirmDischargeTxt.getLocalBounds();
    confirmDischargeTxt.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
    confirmDischargeTxt.setPosition(375.f + 125.f, 320.f + 25.f);

    int reportDocPage = 0;
    int reportPatPage = 0;

    sf::Text reportStats, reportPatHeader, reportDocHeader;
    sf::Text reportPatRows[5], reportDocRows[5];

    reportStats.setFont(Font);
    reportStats.setCharacterSize(20);
    reportStats.setFillColor(sf::Color::Black);
    reportStats.setPosition(190.f, 130.f);

    reportPatHeader.setFont(Font);
    reportPatHeader.setCharacterSize(20);
    reportPatHeader.setFillColor(sf::Color(204, 170, 0));
    reportPatHeader.setString("Patients with Outstanding Bills:");
    reportPatHeader.setPosition(250.f, 260.f);

    reportDocHeader.setFont(Font);
    reportDocHeader.setCharacterSize(20);
    reportDocHeader.setFillColor(sf::Color(100, 220, 255));
    reportDocHeader.setString("Doctor Performance Today:");
    reportDocHeader.setPosition(250.f, 420.f);

    for (int i = 0; i < 5; i++) {
        reportPatRows[i].setFont(Font);
        reportPatRows[i].setCharacterSize(17);
        reportPatRows[i].setFillColor(sf::Color::White);
        reportPatRows[i].setPosition(270.f, 290.f + (i * 24.f));

        reportDocRows[i].setFont(Font);
        reportDocRows[i].setCharacterSize(17);
        reportDocRows[i].setFillColor(sf::Color::White);
        reportDocRows[i].setPosition(270.f, 450.f + (i * 24.f));
    }
    sf::RectangleShape reportPatNextBtn(sf::Vector2f(70.f, 28.f));
    reportPatNextBtn.setFillColor(sf::Color::Green);
    reportPatNextBtn.setPosition(720.f, 358.f);

    sf::RectangleShape reportPatPrevBtn(sf::Vector2f(75.f, 28.f));
    reportPatPrevBtn.setFillColor(sf::Color(150, 150, 150));
    reportPatPrevBtn.setPosition(194.f, 358.f);

    sf::RectangleShape reportDocNextBtn(sf::Vector2f(70.f, 28.f));
    reportDocNextBtn.setFillColor(sf::Color::Green);
    reportDocNextBtn.setPosition(720.f, 520.f);

    sf::RectangleShape reportDocPrevBtn(sf::Vector2f(75.f, 28.f));
    reportDocPrevBtn.setFillColor(sf::Color(150, 150, 150));
    reportDocPrevBtn.setPosition(194.f, 520.f);

    sf::Text rPatNextTxt, rPatPrevTxt, rDocNextTxt, rDocPrevTxt;
    rPatNextTxt.setFont(Font); rPatNextTxt.setCharacterSize(16);
    rPatNextTxt.setString("Next >"); rPatNextTxt.setPosition(724.f, 362.f);
    rPatPrevTxt.setFont(Font); rPatPrevTxt.setCharacterSize(16);
    rPatPrevTxt.setString("< Prev"); rPatPrevTxt.setPosition(190.f, 362.f);
    rDocNextTxt.setFont(Font); rDocNextTxt.setCharacterSize(16);
    rDocNextTxt.setString("Next >"); rDocNextTxt.setPosition(724.f, 524.f);
    rDocPrevTxt.setFont(Font); rDocPrevTxt.setCharacterSize(16);
    rDocPrevTxt.setString("< Prev"); rDocPrevTxt.setPosition(190.f, 524.f);

    int docApptPage = 0, histPage = 0;

    sf::Text daHeader; daHeader.setFont(Font); daHeader.setCharacterSize(22); daHeader.setFillColor(sf::Color(204, 170, 0));
    daHeader.setString("Appt-ID Patient Name  Time   Status"); daHeader.setPosition(200.f, 110.f);

    sf::Text docApptCol1[10], docApptCol2[10], docApptCol3[10], docApptCol4[10];

    sf::Text docHistCol1[5], docHistCol2[5], docHistCol3[5];

    for (int i = 0; i < 10; i++) {
        float yPos = 160.f + (i * 35.f);
        docApptCol1[i].setFont(Font); docApptCol1[i].setCharacterSize(18); docApptCol1[i].setPosition(230.f, yPos); docApptCol1[i].setFillColor(sf::Color::Black);
        docApptCol2[i].setFont(Font); docApptCol2[i].setCharacterSize(18); docApptCol2[i].setPosition(350.f, yPos); docApptCol2[i].setFillColor(sf::Color::Black);
        docApptCol3[i].setFont(Font); docApptCol3[i].setCharacterSize(18); docApptCol3[i].setPosition(600.f, yPos); docApptCol3[i].setFillColor(sf::Color::Black);
        docApptCol4[i].setFont(Font); docApptCol4[i].setCharacterSize(18); docApptCol4[i].setPosition(720.f, yPos); docApptCol4[i].setFillColor(sf::Color::Black);
    }

    for (int i = 0; i < 4; i++) {
        float yPos = 350.f + (i * 35.f);
        docHistCol1[i].setFont(Font); docHistCol1[i].setCharacterSize(16); docHistCol1[i].setPosition(200.f, yPos); docHistCol1[i].setFillColor(sf::Color::Black);
        docHistCol2[i].setFont(Font); docHistCol2[i].setCharacterSize(16); docHistCol2[i].setPosition(370.f, yPos); docHistCol2[i].setFillColor(sf::Color::Black);
        docHistCol3[i].setFont(Font); docHistCol3[i].setCharacterSize(16); docHistCol3[i].setPosition(680.f, yPos); docHistCol3[i].setFillColor(sf::Color::Black);
    }

    string dActionInput = ""; bool activeDActionInput = false;
    sf::RectangleShape dActionBox(sf::Vector2f(200.f, 40.f)); dActionBox.setPosition(500.f, 250.f); dActionBox.setFillColor(sf::Color(150, 150, 150));
    sf::Text dActionDisplay; dActionDisplay.setFont(Font); dActionDisplay.setCharacterSize(22); dActionDisplay.setFillColor(sf::Color::Black); dActionDisplay.setPosition(510.f, 255.f);
    sf::Text dActionLabel; dActionLabel.setFont(Font); dActionLabel.setCharacterSize(21); dActionLabel.setFillColor(sf::Color::Black); dActionLabel.setPosition(250.f, 255.f);
    sf::Text dActionMsg; dActionMsg.setFont(Font); dActionMsg.setCharacterSize(20); dActionMsg.setPosition(300.f, 400.f);
    sf::Text presActionMsg; presActionMsg.setFont(Font); presActionMsg.setCharacterSize(20); presActionMsg.setPosition(400.f, 100.f);

    sf::RectangleShape docActionBtn(sf::Vector2f(240.f, 60.f));
    docActionBtn.setFillColor(sf::Color(200, 50, 50));
    docActionBtn.setPosition(350.f, 320.f);

    sf::Text docActionBtnTxt;
    docActionBtnTxt.setFont(Font);
    docActionBtnTxt.setCharacterSize(25);
    docActionBtnTxt.setFillColor(sf::Color::White);
    docActionBtnTxt.setString("CONFIRM");
    docActionBtnTxt.setPosition(410.f, 332.f);

    string rxInputs[3] = { "", "", "" }; int activeRx = -1;
    sf::RectangleShape rxBoxes[3]; sf::Text rxDisplays[3]; sf::Text rxLabels[3];
    string rxLabelsStr[3] = { "Appt ID:", "Med Dosage :", "Notes:" };
    for (int i = 0; i < 3; i++) {
        rxBoxes[i].setSize(sf::Vector2f(450.f, 40.f)); rxBoxes[i].setFillColor(sf::Color(150, 150, 150)); rxBoxes[i].setPosition(400.f, 150.f + (i * 80.f));
        rxLabels[i].setFont(Font); rxLabels[i].setFillColor(sf::Color::Black); rxLabels[i].setCharacterSize(20); rxLabels[i].setString(rxLabelsStr[i]); rxLabels[i].setPosition(200.f, 155.f + (i * 80.f));
        rxDisplays[i].setFont(Font); rxDisplays[i].setCharacterSize(18); rxDisplays[i].setFillColor(sf::Color::Black); rxDisplays[i].setPosition(410.f, 160.f + (i * 80.f));
    }

    sf::Text hCol1[5], hCol2[5], hCol3[5]; int currentHistPatID = -1;
    for (int i = 0; i < 4; i++) {
        float yPos = 350.f + (i * 35.f);
        hCol1[i].setFont(Font); hCol1[i].setCharacterSize(16); hCol1[i].setPosition(200.f, yPos);
        hCol2[i].setFont(Font); hCol2[i].setCharacterSize(16); hCol2[i].setPosition(370.f, yPos);
        hCol3[i].setFont(Font); hCol3[i].setCharacterSize(16); hCol3[i].setPosition(680.f, yPos);
        hCol1[i].setFillColor(sf::Color::Black); hCol2[i].setFillColor(sf::Color::Black); hCol3[i].setFillColor(sf::Color::Black);
    }
    sf::RectangleShape histLoadBtn(sf::Vector2f(120.f, 45.f));
    histLoadBtn.setFillColor(sf::Color(0, 100, 200));
    histLoadBtn.setPosition(730.f, 250.f);
    sf::Text histLoadTxt; histLoadTxt.setFont(Font); histLoadTxt.setCharacterSize(20);
    histLoadTxt.setString("LOAD"); histLoadTxt.setFillColor(sf::Color::White); histLoadTxt.setPosition(760.f, 260.f);

    int patFailAttempts = 0;

    string patInputs[4] = { "", "", "", "" }; int activePatInput = -1;
    sf::RectangleShape patBoxes[4]; sf::Text patDisplays[4]; sf::Text patLabels[4];
    string patLabelsStr[4] = { "Specialty:", "Doctor ID:", "Date (DD-MM-YYYY):", "Time (HH:MM):" };
    for (int i = 0; i < 4; i++) {
        patBoxes[i].setSize(sf::Vector2f(250.f, 35.f)); patBoxes[i].setFillColor(sf::Color(150, 150, 150)); patBoxes[i].setPosition(450.f, 150.f + (i * 60.f));
        patLabels[i].setFont(Font); patLabels[i].setFillColor(sf::Color::Black); patLabels[i].setCharacterSize(17); patLabels[i].setString(patLabelsStr[i]); patLabels[i].setPosition(190.f, 155.f + (i * 60.f));
        patDisplays[i].setFont(Font); patDisplays[i].setCharacterSize(18); patDisplays[i].setFillColor(sf::Color::Black); patDisplays[i].setPosition(460.f, 155.f + (i * 60.f));
    }

    string singlePatInput = ""; bool activeSinglePatInput = false;
    sf::RectangleShape singlePatBox(sf::Vector2f(200.f, 40.f)); singlePatBox.setPosition(500.f, 250.f); singlePatBox.setFillColor(sf::Color(150, 150, 150));
    sf::Text singlePatDisplay; singlePatDisplay.setFont(Font); singlePatDisplay.setCharacterSize(22); singlePatDisplay.setFillColor(sf::Color::Black); singlePatDisplay.setPosition(510.f, 255.f);
    sf::Text singlePatLabel; singlePatLabel.setFont(Font); singlePatLabel.setCharacterSize(22); singlePatLabel.setFillColor(sf::Color::Black); singlePatLabel.setPosition(280.f, 245.f);

    sf::RectangleShape bookBtn(sf::Vector2f(340.f, 50.f));
    bookBtn.setFillColor(sf::Color(50, 200, 50));
    bookBtn.setPosition(450.f, 400.f);

    sf::Text bookBtnTxt; bookBtnTxt.setFont(Font); bookBtnTxt.setCharacterSize(22);
    bookBtnTxt.setString("BOOK APPOINTMENT"); bookBtnTxt.setFillColor(sf::Color::White);
    bookBtnTxt.setPosition(460.f, 410.f);

    sf::Text bookMsg; bookMsg.setFont(Font); bookMsg.setCharacterSize(20);

    sf::RectangleShape singleActionBtn(sf::Vector2f(200.f, 50.f));
    singleActionBtn.setFillColor(sf::Color(50, 200, 50));
    singleActionBtn.setPosition(500.f, 320.f);

    sf::Text singleActionBtnTxt; singleActionBtnTxt.setFont(Font); singleActionBtnTxt.setCharacterSize(22);
    singleActionBtnTxt.setFillColor(sf::Color::White);

    sf::Text singleActionMsg; singleActionMsg.setFont(Font); singleActionMsg.setCharacterSize(20);
    sf::Text totalBillsMsg; totalBillsMsg.setFont(Font); totalBillsMsg.setCharacterSize(20);

    sf::Text patViewApptCol1[10], patViewApptCol2[10], patViewApptCol3[10];
    sf::Text patViewApptCol4[10], patViewApptCol5[10], patViewApptCol6[10];

    sf::Text patViewBillCol1[10], patViewBillCol2[10], patViewBillCol3[10];
    sf::Text patViewBillCol4[10], patViewBillCol5[10];

    sf::Text patViewRecCol1[10], patViewRecCol2[10], patViewRecCol3[10], patViewRecCol4[10];

    sf::Text patViewApptHeader, patViewBillHeader, patViewRecHeader;
    sf::Text patTotalBillsMsg;

    for (int i = 0; i < 10; i++) {
        float yPos = 160.f + (i * 35.f);
        patViewApptCol1[i].setFont(Font); patViewApptCol1[i].setCharacterSize(16); patViewApptCol1[i].setPosition(135.f, yPos); patViewApptCol1[i].setFillColor(sf::Color::Black); // ID
        patViewApptCol2[i].setFont(Font); patViewApptCol2[i].setCharacterSize(16); patViewApptCol2[i].setPosition(200.f, yPos); patViewApptCol2[i].setFillColor(sf::Color::Black); // Doctor Name
        patViewApptCol3[i].setFont(Font); patViewApptCol3[i].setCharacterSize(16); patViewApptCol3[i].setPosition(410.f, yPos); patViewApptCol3[i].setFillColor(sf::Color::Black); // Specialty
        patViewApptCol4[i].setFont(Font); patViewApptCol4[i].setCharacterSize(16); patViewApptCol4[i].setPosition(570.f, yPos); patViewApptCol4[i].setFillColor(sf::Color::Black); // Date
        patViewApptCol5[i].setFont(Font); patViewApptCol5[i].setCharacterSize(16); patViewApptCol5[i].setPosition(730.f, yPos); patViewApptCol5[i].setFillColor(sf::Color::Black); // Time
        patViewApptCol6[i].setFont(Font); patViewApptCol6[i].setCharacterSize(16); patViewApptCol6[i].setPosition(850.f, yPos); patViewApptCol6[i].setFillColor(sf::Color::Black); // Status

        patViewBillCol1[i].setFont(Font); patViewBillCol1[i].setCharacterSize(18); patViewBillCol1[i].setPosition(195.f, yPos); patViewBillCol1[i].setFillColor(sf::Color::Black); // Bill ID
        patViewBillCol2[i].setFont(Font); patViewBillCol2[i].setCharacterSize(18); patViewBillCol2[i].setPosition(270.f, yPos); patViewBillCol2[i].setFillColor(sf::Color::Black); // Appt ID
        patViewBillCol3[i].setFont(Font); patViewBillCol3[i].setCharacterSize(18); patViewBillCol3[i].setPosition(380.f, yPos); patViewBillCol3[i].setFillColor(sf::Color::Black); // Amount
        patViewBillCol4[i].setFont(Font); patViewBillCol4[i].setCharacterSize(18); patViewBillCol4[i].setPosition(530.f, yPos); patViewBillCol4[i].setFillColor(sf::Color::Black); // Status
        patViewBillCol5[i].setFont(Font); patViewBillCol5[i].setCharacterSize(18); patViewBillCol5[i].setPosition(700.f, yPos); patViewBillCol5[i].setFillColor(sf::Color::Black); // Date

        patViewRecCol1[i].setFont(Font); patViewRecCol1[i].setCharacterSize(16); patViewRecCol1[i].setPosition(135.f, yPos); patViewRecCol1[i].setFillColor(sf::Color::Black); // Date
        patViewRecCol2[i].setFont(Font); patViewRecCol2[i].setCharacterSize(14); patViewRecCol2[i].setPosition(310.f, yPos); patViewRecCol2[i].setFillColor(sf::Color::Black); // Doctor
        patViewRecCol3[i].setFont(Font); patViewRecCol3[i].setCharacterSize(14); patViewRecCol3[i].setPosition(480.f, yPos); patViewRecCol3[i].setFillColor(sf::Color::Black); // Medicines
        patViewRecCol4[i].setFont(Font); patViewRecCol4[i].setCharacterSize(14); patViewRecCol4[i].setPosition(750.f, yPos); patViewRecCol4[i].setFillColor(sf::Color::Black); // Notes
    }
    patViewApptHeader.setFont(Font); patViewApptHeader.setCharacterSize(17);
    patViewApptHeader.setFillColor(sf::Color(204, 170, 0));
    patViewApptHeader.setString("ID    Doctor Name   Specialty     Date     Time    Status");
    patViewApptHeader.setPosition(135.f, 110.f);

    patViewBillHeader.setFont(Font); patViewBillHeader.setCharacterSize(18);
    patViewBillHeader.setFillColor(sf::Color(204, 170, 0));
    patViewBillHeader.setString("BillID  ApptID  Amount    Status       Date");
    patViewBillHeader.setPosition(155.f, 110.f);

    patViewRecHeader.setFont(Font); patViewRecHeader.setCharacterSize(18);
    patViewRecHeader.setFillColor(sf::Color(204, 170, 0));
    patViewRecHeader.setString("Date     Doctor Name    Medicines           Notes");
    patViewRecHeader.setPosition(155.f, 110.f);

    patTotalBillsMsg.setFont(Font); patTotalBillsMsg.setCharacterSize(18);
    patTotalBillsMsg.setFillColor(sf::Color::Yellow);
    patTotalBillsMsg.setPosition(300.f, 50.f);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8 || (event.text.unicode >= 32 && event.text.unicode < 128)) {
                    showError = false;
                }
                if (isPassActive) {
                    if (event.text.unicode == 8) {
                        if (passCharCount > 0) {
                            passCharCount--;
                            passBuffer[passCharCount] = '\0';
                        }
                    }
                    else if (event.text.unicode >= 32 && event.text.unicode < 128 && passCharCount < 31) {
                        passBuffer[passCharCount] = static_cast<char>(event.text.unicode);
                        passCharCount++;
                        passBuffer[passCharCount] = '\0';
                    }
                }
                else if (isIdActive) {
                    if (event.text.unicode == 8) {
                        if (idCharCount > 0) {
                            idCharCount--;
                            idBuffer[idCharCount] = '\0';
                        }
                    }
                    else if (event.text.unicode >= 32 && event.text.unicode < 128 && idCharCount < 31) {
                        idBuffer[idCharCount] = static_cast<char>(event.text.unicode);
                        idCharCount++;
                        idBuffer[idCharCount] = '\0';
                    }
                }
                if (currentState == STATE_ADMIN_ADD_DOCTOR) {
                    for (int i = 0; i < 6; i++) {
                        if (activeDoc[i]) {
                            if (event.text.unicode == 8 && docInput[i].length() > 0) {
                                docInput[i].pop_back();
                            }
                            else if (event.text.unicode >= 32 && event.text.unicode < 128 && docInput[i].length() < 25) {
                                docInput[i] += static_cast<char>(event.text.unicode);
                            }
                            docDisplay[i].setString(docInput[i]);
                        }
                    }
                }
                else if (currentState == STATE_ADMIN_REMOVE_DOCTOR) {
                    if (activeRemoveDoc) {
                        if (event.text.unicode == 8 && removeDocInput.length() > 0) {
                            removeDocInput.pop_back();
                        }
                        else if (event.text.unicode >= 48 && event.text.unicode <= 57 && removeDocInput.length() < 10) {
                            removeDocInput += static_cast<char>(event.text.unicode);
                        }
                        removeDocDisplay.setString(removeDocInput);
                    }
                }
                else if (currentState == STATE_DOCTOR_MARK_COMPLETE || currentState == STATE_DOCTOR_MARK_NO_SHOW || currentState == STATE_DOCTOR_VIEW_HISTORY) {
                    if (activeDActionInput) {
                        if (event.text.unicode == 8 && !dActionInput.empty()) dActionInput.pop_back();
                        else if (event.text.unicode >= '0' && event.text.unicode <= '9' && dActionInput.length() < 6)
                            dActionInput += static_cast<char>(event.text.unicode);
                        dActionDisplay.setString(dActionInput);
                    }
                }
                else if (currentState == STATE_DOCTOR_WRITE_PRESCRIPTION && activeRx != -1) {
                    if (event.text.unicode == 8 && !rxInputs[activeRx].empty()) rxInputs[activeRx].pop_back();
                    else if (event.text.unicode >= 32 && event.text.unicode < 128) {
                        if ((activeRx == 0 && rxInputs[0].length() < 6 && event.text.unicode >= '0' && event.text.unicode <= '9') ||
                            (activeRx == 1 && rxInputs[1].length() < 49) ||
                            (activeRx == 2 && rxInputs[2].length() < 55)) {
                            rxInputs[activeRx] += static_cast<char>(event.text.unicode);
                        }
                    }
                    rxDisplays[activeRx].setString(rxInputs[activeRx]);
                }
                else if (currentState == STATE_PATIENT_CANCEL_APPOINTMENT || currentState == STATE_PATIENT_PAY_BILL || currentState == STATE_PATIENT_TOP_UP_BALANCE) {
                    if (activeSinglePatInput) {
                        if (event.text.unicode == 8 && !singlePatInput.empty()) singlePatInput.pop_back();
                        else if (event.text.unicode >= '0' && event.text.unicode <= '9' && singlePatInput.length() < 8)
                            singlePatInput += static_cast<char>(event.text.unicode);
                        singlePatDisplay.setString(singlePatInput);
                    }
                }
                else if (currentState == STATE_PATIENT_BOOK_APPOINTMENT && activePatInput != -1) {
                    if (event.text.unicode == 8 && !patInputs[activePatInput].empty()) patInputs[activePatInput].pop_back();
                    else if (event.text.unicode >= 32 && event.text.unicode < 128) {
                        if (patInputs[activePatInput].length() < 20) patInputs[activePatInput] += static_cast<char>(event.text.unicode);
                    }
                    patDisplays[activePatInput].setString(patInputs[activePatInput]);
                }
                if (currentState == STATE_ADMIN_DISCHARGE_PATIENT && activeDischargeInput) {
                    if (event.text.unicode == 8) {
                        if (!dischargeInput.empty()) dischargeInput.pop_back();
                    }
                    else if (event.text.unicode >= 48 && event.text.unicode <= 57) {
                        if (dischargeInput.length() < 6) {
                            dischargeInput += static_cast<char>(event.text.unicode);
                        }
                    }
                    dischargeDisplay.setString(dischargeInput);
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePosF(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                clickAudio.play();
                if (currentState == STATE_HOME) {
                    if (bgTextBox.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_LOGIN_SELECTION;
                    }
                }
                else if (currentState == STATE_LOGIN_SELECTION) {
                    if (loginTextBox1.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_ADMIN_LOGIN;
                    }
                    else if (loginTextBox2.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_DOCTOR_LOGIN;
                    }
                    else if (loginTextBox3.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_PATIENT_LOGIN;
                    }
                }
                else if (currentState == STATE_ADMIN_LOGIN || currentState == STATE_PATIENT_LOGIN || currentState == STATE_DOCTOR_LOGIN) {
                    if (portalBackBtn.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_LOGIN_SELECTION;
                        idBuffer[0] = '\0'; idCharCount = 0;
                        passBuffer[0] = '\0'; passCharCount = 0;
                        isIdActive = false; isPassActive = false;
                        showError = false;
                    }
                    else if (portalTextBox1.getGlobalBounds().contains(mousePosF)) {
                        isIdActive = true;
                        isPassActive = false;
                    }
                    else if (portalTextBox2.getGlobalBounds().contains(mousePosF)) {
                        isPassActive = true;
                        isIdActive = false;
                    }
                }
                else if (currentState == STATE_ADMIN_DASHBOARD) {
                    if (adminTextBox1.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_ADMIN_ADD_DOCTOR;
                    }
                    else if (adminTextBox2.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_ADMIN_REMOVE_DOCTOR;
                    }
                    else if (adminTextBox3.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_ADMIN_VIEW_PATIENTS;
                        currentPatPage = 0;
                        refreshPatientList(pCol1, pCol2, pCol3, pCol4, pCol5, pCol6, currentPatPage);
                    }
                    else if (adminTextBox4.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_ADMIN_VIEW_DOCTORS;
                        currentDocPage = 0;
                        refreshDoctorList(colID, colName, colSpec, colFee, currentDocPage);
                    }
                    else if (adminTextBox5.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_ADMIN_VIEW_APPOINTMENTS;
                        currentApptPage = 0;
                        refreshAppointments(aCol1, aCol2, aCol3, aCol4, aCol5, aCol6, currentApptPage);
                    }
                    else if (adminTextBox6.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_ADMIN_VIEW_BILLS;
                        currentBillPage = 0;
                        refreshUnpaidBills(bCol1, bCol2, bCol3, bCol4, currentBillPage);
                    }
                    else if (adminTextBox7.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_ADMIN_DISCHARGE_PATIENT;
                    }
                    else if (adminTextBox8.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_ADMIN_VIEW_SECURITY_LOG;
                        currentLogPage = 0;
                        refreshSecurityLog(logLines, currentLogPage);
                    }
                    else if (adminTextBox9.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_ADMIN_GENERATE_REPORT;
                        refreshDailyReport(reportStats, reportPatRows, reportDocRows, reportPatPage, reportDocPage);
                    }
                    else if (adminTextBox10.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_HOME;
                        idBuffer[0] = '\0'; idCharCount = 0;
                        passBuffer[0] = '\0'; passCharCount = 0;
                    }
                }
                else if (currentState == STATE_ADMIN_VIEW_DOCTORS) {
                    if (nextBtn.getGlobalBounds().contains(mousePosF)) {
                        Storage<Doctor>* temp = new Storage<Doctor>();
                        FileHandler::loadDoctors(*temp);
                        if ((currentDocPage + 1) * 10 < temp->getCount()) {
                            currentDocPage++;
                            refreshDoctorList(colID, colName, colSpec, colFee, currentDocPage);
                        }
                        delete temp;
                    }
                    else if (prevBtn.getGlobalBounds().contains(mousePosF) && currentDocPage > 0) {
                        currentDocPage--;
                        refreshDoctorList(colID, colName, colSpec, colFee, currentDocPage);
                    }
                    else if (backBtn.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_ADMIN_DASHBOARD;
                    }
                }
                else if (currentState == STATE_ADMIN_ADD_DOCTOR) {
                    if (backBtn.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_ADMIN_DASHBOARD;
                    }
                    else if (saveBtn.getGlobalBounds().contains(mousePosF)) {

                        bool allFilled = true;
                        for (int i = 0; i < 6; i++) {
                            if (docInput[i].empty()) allFilled = false;
                        }

                        if (allFilled) {
                            int newId = stoi(docInput[0]);
                            float newFee = stof(docInput[5]);
                            Doctor newDoc(newId, docInput[1].c_str(), docInput[2].c_str(), docInput[3].c_str(), docInput[4].c_str(), newFee);

                            FileHandler fh;
                            fh.addDoctor(newDoc);
                            for (int i = 0; i < 6; i++) { docInput[i] = ""; docDisplay[i].setString(""); }

                            currentState = STATE_ADMIN_DASHBOARD;
                        }
                        else {
                            addDocErrorMsg.setString("Error: You must fill out all 6 fields!");
                            showAddDocError = true;
                        }
                    }
                    else {
                        for (int i = 0; i < 6; i++) {
                            if (docBoxes[i].getGlobalBounds().contains(mousePosF)) {
                                activeDoc[i] = true;
                                docBoxes[i].setFillColor(sf::Color(255, 255, 255));
                            }
                            else {
                                activeDoc[i] = false;
                                docBoxes[i].setFillColor(sf::Color(150, 150, 150));
                            }
                        }
                    }
                }
                else if (currentState == STATE_ADMIN_REMOVE_DOCTOR) {
                    if (backBtn.getGlobalBounds().contains(mousePosF)) {
                        removeDocStatusMsg.setString("");
                        removeDocInput = "";
                        removeDocDisplay.setString("");
                        currentState = STATE_ADMIN_DASHBOARD;
                    }
                    else if (confirmRemoveBtn.getGlobalBounds().contains(mousePosF)) {
                        if (!removeDocInput.empty()) {

                            int idToRemove = stoi(removeDocInput);
                            Storage<Doctor>* docStorage = new Storage<Doctor>();

                            FileHandler::loadDoctors(*docStorage);
                            bool success = docStorage->removeByID(idToRemove);

                            if (success) {
                                FileHandler::deleteDoctor(*docStorage);

                                removeDocStatusMsg.setFillColor(sf::Color::Green);
                                removeDocStatusMsg.setString("Success: Doctor " + removeDocInput + " removed.");
                            }
                            else {
                                removeDocStatusMsg.setFillColor(sf::Color::Red);
                                removeDocStatusMsg.setString("Error: ID not found in database!");
                            }
                            delete docStorage;
                            removeDocInput = "";
                            removeDocDisplay.setString("");
                        }
                        else {
                            removeDocStatusMsg.setFillColor(sf::Color::Red);
                            removeDocStatusMsg.setString("Error: Please enter an ID.");
                        }
                    }
                    else if (removeDocBox.getGlobalBounds().contains(mousePosF)) {
                        activeRemoveDoc = true;
                        removeDocBox.setFillColor(sf::Color(255, 255, 255));
                    }
                    else {
                        activeRemoveDoc = false;
                        removeDocBox.setFillColor(sf::Color(150, 150, 150));
                    }
                }
                else if (currentState == STATE_ADMIN_VIEW_PATIENTS) {
                    bool changed = false;
                    if (nextBtn.getGlobalBounds().contains(mousePosF)) {
                        Storage<Patient>* temp = new Storage<Patient>();
                        FileHandler::loadPatients(*temp);
                        if ((currentPatPage + 1) * 10 < temp->getCount()) {
                            currentPatPage++;
                            changed = true;
                        }
                        delete temp;
                    }
                    else if (prevBtn.getGlobalBounds().contains(mousePosF) && currentPatPage > 0) {
                        currentPatPage--;
                        changed = true;
                    }

                    if (changed) {
                        refreshPatientList(pCol1, pCol2, pCol3, pCol4, pCol5, pCol6, currentPatPage);
                    }
                    if (backBtn.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_ADMIN_DASHBOARD;
                    }
                }
                else if (currentState == STATE_ADMIN_VIEW_APPOINTMENTS) {
                    bool changed = false;
                    if (nextBtn.getGlobalBounds().contains(mousePosF)) {
                        Storage<Appointment>* temp = new Storage<Appointment>();
                        FileHandler::loadAppointments(*temp);
                        if ((currentApptPage + 1) * 10 < temp->getCount()) {
                            currentApptPage++;
                            changed = true;
                        }
                        delete temp;
                    }
                    else if (prevBtn.getGlobalBounds().contains(mousePosF) && currentApptPage > 0) {
                        currentApptPage--;
                        changed = true;
                    }
                    if (changed) {
                        refreshAppointments(aCol1, aCol2, aCol3, aCol4, aCol5, aCol6, currentApptPage);
                    }
                    if (backBtn.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_ADMIN_DASHBOARD;
                    }
                }
                else if (currentState == STATE_ADMIN_VIEW_SECURITY_LOG) {
                    bool changed = false;

                    if (nextBtn.getGlobalBounds().contains(mousePosF)) {
                        int totalLines = 0;
                        string temp;
                        ifstream file("security_log.txt");
                        if (file.is_open()) {
                            while (getline(file, temp)) {
                                totalLines++;
                            }
                            file.close();
                        }
                        if ((currentLogPage + 1) * 12 < totalLines) {
                            currentLogPage++;
                            changed = true;
                        }
                    }
                    else if (prevBtn.getGlobalBounds().contains(mousePosF) && currentLogPage > 0) {
                        currentLogPage--;
                        changed = true;
                    }
                    if (changed) {
                        refreshSecurityLog(logLines, currentLogPage);
                    }
                    if (backBtn.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_ADMIN_DASHBOARD;
                    }
                }
                else if (currentState == STATE_ADMIN_VIEW_BILLS) {
                    if (nextBtn.getGlobalBounds().contains(mousePosF)) {
                        Storage<Bill>* temp = new Storage<Bill>();
                        FileHandler::loadBills(*temp);
                        int totalUnpaid = 0;
                        for (int i = 0; i < temp->getCount(); i++) {
                            if (string(temp->getAt(i).getStatus()) == "unpaid") totalUnpaid++;
                        }
                        if ((currentBillPage + 1) * 10 < totalUnpaid) {
                            currentBillPage++;
                            refreshUnpaidBills(bCol1, bCol2, bCol3, bCol4, currentBillPage);
                        }
                        delete temp;
                    }
                    else if (prevBtn.getGlobalBounds().contains(mousePosF) && currentBillPage > 0) {
                        currentBillPage--;
                        refreshUnpaidBills(bCol1, bCol2, bCol3, bCol4, currentBillPage);
                    }
                    else if (backBtn.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_ADMIN_DASHBOARD;
                    }
                }
                else if (currentState == STATE_ADMIN_DISCHARGE_PATIENT) {
                    sf::Vector2f mousePosF(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

                    if (backBtn.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_ADMIN_DASHBOARD;
                        dischargeInput = "";
                        dischargeDisplay.setString("");
                        dischargeStatusMsg.setString("");
                        activeDischargeInput = false;
                    }
                    else if (dischargeBox.getGlobalBounds().contains(mousePosF)) {
                        activeDischargeInput = true;
                        dischargeBox.setOutlineColor(sf::Color::Yellow);
                    }
                    else if (confirmDischargeBtn.getGlobalBounds().contains(mousePosF) && !dischargeInput.empty()) {
                        int pID = stoi(dischargeInput);
                        bool success = FileHandler::dischargePatient(pID);

                        if (success) {
                            dischargeStatusMsg.setFillColor(sf::Color::Green);
                            dischargeStatusMsg.setString("Success: Patient archived and\n records cleared.");
                        }
                        else {
                            dischargeStatusMsg.setFillColor(sf::Color::Red);
                            dischargeStatusMsg.setString("Error: Active bills/appointments\n or invalid ID.");
                        }
                        dischargeInput = "";
                        dischargeDisplay.setString("");
                        activeDischargeInput = false;
                        dischargeBox.setOutlineColor(sf::Color::Black);
                    }
                    else {
                        activeDischargeInput = false;
                        dischargeBox.setOutlineColor(sf::Color::Black);
                    }
                }
                else if (currentState == STATE_ADMIN_GENERATE_REPORT) {
                    if (backBtn.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_ADMIN_DASHBOARD;
                        reportPatPage = 0;
                        reportDocPage = 0;
                    }
                    else if (reportPatNextBtn.getGlobalBounds().contains(mousePosF)) {
                        reportPatPage++;
                        refreshDailyReport(reportStats, reportPatRows, reportDocRows, reportPatPage, reportDocPage);
                    }
                    else if (reportPatPrevBtn.getGlobalBounds().contains(mousePosF) && reportPatPage > 0) {
                        reportPatPage--;
                        refreshDailyReport(reportStats, reportPatRows, reportDocRows, reportPatPage, reportDocPage);
                    }
                    else if (reportDocNextBtn.getGlobalBounds().contains(mousePosF)) {
                        reportDocPage++;
                        refreshDailyReport(reportStats, reportPatRows, reportDocRows, reportPatPage, reportDocPage);
                    }
                    else if (reportDocPrevBtn.getGlobalBounds().contains(mousePosF) && reportDocPage > 0) {
                        reportDocPage--;
                        refreshDailyReport(reportStats, reportPatRows, reportDocRows, reportPatPage, reportDocPage);
                    }
                }
//-----------------------------------------------------------------------------------------------------------------------------------------
                else if (currentState == STATE_DOCTOR_DASHBOARD) {
                    if (doctorTextBox1.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_DOCTOR_VIEW_APPOINTMENTS;
                        docApptPage = 0; refreshDoctorAppointments(docApptCol1, docApptCol2, docApptCol3, docApptCol4, loggedInUserID, docApptPage);
                    }
                    else if (doctorTextBox2.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_DOCTOR_MARK_COMPLETE;
                        dActionLabel.setString("Enter Appt ID\nto Complete:"); dActionInput = ""; dActionDisplay.setString(""); dActionMsg.setString("");
                    }
                    else if (doctorTextBox3.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_DOCTOR_MARK_NO_SHOW;
                        dActionLabel.setString("Enter Appt ID\n for No-Show:"); dActionInput = ""; dActionDisplay.setString(""); dActionMsg.setString("");
                    }
                    else if (doctorTextBox4.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_DOCTOR_WRITE_PRESCRIPTION;
                        for (int i = 0; i < 3; i++) { rxInputs[i] = ""; rxDisplays[i].setString(""); rxBoxes[i].setFillColor(sf::Color(150, 150, 150)); }
                        activeRx = -1; dActionMsg.setString("");
                    }
                    else if (doctorTextBox5.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_DOCTOR_VIEW_HISTORY;
                        dActionLabel.setString("Enter Patient \nID to view:"); dActionInput = ""; dActionDisplay.setString(""); dActionMsg.setString("");
                        currentHistPatID = -1; for (int i = 0; i < 4; i++) { docHistCol1[i].setString(""); docHistCol2[i].setString(""); docHistCol3[i].setString(""); }
                    }
                    else if (doctorTextBox6.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_HOME;
                        idBuffer[0] = '\0'; idCharCount = 0;
                        passBuffer[0] = '\0'; passCharCount = 0;
                        loggedInUserID = -1;
                    }
                }
                else if (currentState == STATE_DOCTOR_VIEW_APPOINTMENTS) {
                        if (nextBtn.getGlobalBounds().contains(mousePosF)) { docApptPage++; refreshDoctorAppointments(docApptCol1, docApptCol2, docApptCol3, docApptCol4, loggedInUserID, docApptPage);}
                        else if (prevBtn.getGlobalBounds().contains(mousePosF) && docApptPage > 0) { docApptPage--; refreshDoctorAppointments(docApptCol1, docApptCol2, docApptCol3, docApptCol4, loggedInUserID, docApptPage);}
                        else if (backBtn.getGlobalBounds().contains(mousePosF)) currentState = STATE_DOCTOR_DASHBOARD;
                        }
                else if (currentState == STATE_DOCTOR_MARK_COMPLETE || currentState == STATE_DOCTOR_MARK_NO_SHOW) {
                    if (backBtn.getGlobalBounds().contains(mousePosF)) currentState = STATE_DOCTOR_DASHBOARD;
                    else if (dActionBox.getGlobalBounds().contains(mousePosF)) {
                        activeDActionInput = true;
                        dActionBox.setFillColor(sf::Color(255, 255, 255));
                        dActionBox.setOutlineColor(sf::Color::Yellow);
                    }
                    else if (docActionBtn.getGlobalBounds().contains(mousePosF) && !dActionInput.empty()) {
                        string act = (currentState == STATE_DOCTOR_MARK_COMPLETE) ? "completed" : "no-show";
                        processDoctorAction(loggedInUserID, stoi(dActionInput), act, dActionMsg);
                        dActionInput = ""; dActionDisplay.setString("");
                        activeDActionInput = false;
                        dActionBox.setFillColor(sf::Color(150, 150, 150));
                        dActionBox.setOutlineColor(sf::Color::Black);
                    }
                    else {
                        activeDActionInput = false;
                        dActionBox.setFillColor(sf::Color(150, 150, 150));
                        dActionBox.setOutlineColor(sf::Color::Black);
                    }
                }
                else if (currentState == STATE_DOCTOR_VIEW_HISTORY) {
                    if (backBtn.getGlobalBounds().contains(mousePosF)) currentState = STATE_DOCTOR_DASHBOARD;
                    else if (dActionBox.getGlobalBounds().contains(mousePosF)) {
                        activeDActionInput = true;
                        dActionBox.setFillColor(sf::Color(255, 255, 255));
                        dActionBox.setOutlineColor(sf::Color::Yellow);
                    }
                    else if (histLoadBtn.getGlobalBounds().contains(mousePosF) && !dActionInput.empty()) {
                        currentHistPatID = stoi(dActionInput); histPage = 0;
                        refreshHistory(docHistCol1, docHistCol2, docHistCol3, loggedInUserID, currentHistPatID, histPage, dActionMsg);
                    }
                    else if (docActionBtn.getGlobalBounds().contains(mousePosF) && !dActionInput.empty()) {
                        currentHistPatID = stoi(dActionInput); histPage = 0;
                        refreshHistory(docHistCol1, docHistCol2, docHistCol3, loggedInUserID, currentHistPatID, histPage, dActionMsg);
                    }
                    else if (nextBtn.getGlobalBounds().contains(mousePosF) && currentHistPatID != -1) {
                        histPage++; refreshHistory(docHistCol1, docHistCol2, docHistCol3, loggedInUserID, currentHistPatID, histPage, dActionMsg);
                    }
                    else if (prevBtn.getGlobalBounds().contains(mousePosF) && currentHistPatID != -1 && histPage > 0) {
                        histPage--; refreshHistory(docHistCol1, docHistCol2, docHistCol3, loggedInUserID, currentHistPatID, histPage, dActionMsg);
                    }
                    else {
                        activeDActionInput = false;
                        dActionBox.setFillColor(sf::Color(150, 150, 150));
                        dActionBox.setOutlineColor(sf::Color::Black);
                    }
                }
                else if (currentState == STATE_DOCTOR_WRITE_PRESCRIPTION) {
                        if (backBtn.getGlobalBounds().contains(mousePosF)) currentState = STATE_DOCTOR_DASHBOARD;
                        else if (docActionBtn.getGlobalBounds().contains(mousePosF)) {
                            if (!rxInputs[0].empty() && !rxInputs[1].empty() && !rxInputs[2].empty()) {
                                submitPrescription(loggedInUserID, stoi(rxInputs[0]), rxInputs[1], rxInputs[2], dActionMsg);
                                for (int i = 0; i < 3; i++) {
                                    rxInputs[i] = "";
                                    rxDisplays[i].setString("");
                                    rxBoxes[i].setFillColor(sf::Color(150, 150, 150));
                                    rxBoxes[i].setOutlineColor(sf::Color::Black);
                                }
                            }
                            else {
                                dActionMsg.setString("Please fill all fields.");
                                dActionMsg.setFillColor(sf::Color::Red);
                            }
                        }
                        else {
                            activeRx = -1;
                            for (int i = 0; i < 3; i++) {
                                if (rxBoxes[i].getGlobalBounds().contains(mousePosF)) {
                                    activeRx = i;
                                    rxBoxes[i].setFillColor(sf::Color(255, 255, 255));
                                    rxBoxes[i].setOutlineColor(sf::Color::Yellow);
                                }
                                else {
                                    rxBoxes[i].setFillColor(sf::Color(150, 150, 150));
                                    rxBoxes[i].setOutlineColor(sf::Color::Black);
                                }
                            }
                        }
                }
                else if (currentState == STATE_PATIENT_DASHBOARD) {
                    patFailAttempts = 0;
                    singlePatInput = "";
                    singlePatDisplay.setString("");
                    bookMsg.setString("");
                    singleActionMsg.setString("");
                    for (int i = 0; i < 4; i++) {
                        patInputs[i] = "";
                        patDisplays[i].setString("");
                    }

                    if (patientTextBox1.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_PATIENT_BOOK_APPOINTMENT;
                    }
                    else if (patientTextBox2.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_PATIENT_CANCEL_APPOINTMENT;
                        singlePatLabel.setString("Appt ID\nto Cancel:");
                        singlePatInput = "";
                        singlePatDisplay.setString("");
                        singleActionMsg.setString("");
                        activeSinglePatInput = false;
                    }
                    else if (patientTextBox3.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_PATIENT_VIEW_APPOINTMENTS;
                        currentApptPage = 0;
                        refreshPatAppointments(patViewApptCol1, patViewApptCol2, patViewApptCol3,
                            patViewApptCol4, patViewApptCol5, patViewApptCol6,
                            loggedInUserID, currentApptPage);
                    }
                    else if (patientTextBox4.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_PATIENT_VIEW_RECORDS;
                        currentDocPage = 0;
                        refreshPatRecords(patViewRecCol1, patViewRecCol2, patViewRecCol3,
                            patViewRecCol4, loggedInUserID, currentDocPage);
                    }
                    else if (patientTextBox5.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_PATIENT_VIEW_BILLS;
                        currentBillPage = 0;
                        refreshPatBills(patViewBillCol1, patViewBillCol2, patViewBillCol3,
                            patViewBillCol4, patViewBillCol5,
                            loggedInUserID, currentBillPage, patTotalBillsMsg);
                    }
                    else if (patientTextBox6.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_PATIENT_PAY_BILL;
                        singlePatLabel.setString("Enter Bill\nID to Pay:");
                        singlePatInput = "";
                        singlePatDisplay.setString("");
                        singleActionMsg.setString("");
                        activeSinglePatInput = false;
                    }
                    else if (patientTextBox7.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_PATIENT_TOP_UP_BALANCE;
                        singlePatLabel.setString("Enter\nAmount(PKR):");
                        singlePatInput = "";
                        singlePatDisplay.setString("");
                        singleActionMsg.setString("");
                        activeSinglePatInput = false;
                    }
                    else if (patientTextBox8.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_HOME;
                        loggedInUserID = -1;
                        loggedInUserName = "";
                        idBuffer[0] = '\0'; idCharCount = 0;
                        passBuffer[0] = '\0'; passCharCount = 0;
                    }
                }
                else if (currentState == STATE_PATIENT_BOOK_APPOINTMENT) {
                    if (backBtn.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_PATIENT_DASHBOARD;
                    }
                    else if (bookBtn.getGlobalBounds().contains(mousePosF)) {
                        try {
                            Storage<Doctor> dStore; FileHandler::loadDoctors(dStore);
                            Storage<Appointment> aStore; FileHandler::loadAppointments(aStore);
                            Storage<Patient> pStore; FileHandler::loadPatients(pStore);

                            if (!Validator::isValidDate(patInputs[2].c_str())) {
                                patFailAttempts++;
                                if (patFailAttempts >= 3) {
                                    currentState = STATE_PATIENT_DASHBOARD;
                                    bookMsg.setString("");
                                    patFailAttempts = 0;
                                }
                                else {
                                    bookMsg.setString("Invalid date. Attempt " + to_string(patFailAttempts) + "/3");
                                    bookMsg.setFillColor(sf::Color::Red);
                                }
                                throw InvalidInputException();
                            }

                            if (!Validator::isValidTimeSlot(patInputs[3].c_str())) {
                                bookMsg.setString("Invalid time slot.");
                                bookMsg.setFillColor(sf::Color::Red);
                                throw InvalidInputException();
                            }

                            int dIdx = -1;
                            for (int i = 0; i < dStore.getCount(); i++) {
                                char specBuf[50];
                                int k = 0;
                                while (dStore.getAt(i).getSpecialization()[k] != '\0' && k < 49) {
                                    specBuf[k] = dStore.getAt(i).getSpecialization()[k]; k++;
                                }
                                specBuf[k] = '\0';
                                manualTrim(specBuf);

                                if (to_string(dStore.getAt(i).getID()) == patInputs[1] &&
                                    manualCaseInsensitiveMatch(specBuf, patInputs[0].c_str())) {
                                    dIdx = i; break;
                                }
                            }
                            if (dIdx == -1) {
                                bookMsg.setString("Doctor not found.");
                                bookMsg.setFillColor(sf::Color::Red);
                                throw InvalidInputException();
                            }

                            for (int i = 0; i < aStore.getCount(); i++) {
                                string stat = string(aStore.getAt(i).getStatus());
                                while (!stat.empty() && (isspace(stat.back()) || stat.back() == '\r')) stat.pop_back();
                                string aDate = string(aStore.getAt(i).getDate());
                                while (!aDate.empty() && (isspace(aDate.back()) || aDate.back() == '\r')) aDate.pop_back();
                                string aTime = string(aStore.getAt(i).getTimeSlot());
                                while (!aTime.empty() && (isspace(aTime.back()) || aTime.back() == '\r')) aTime.pop_back();

                                if (aStore.getAt(i).getDoctorID() == dStore.getAt(dIdx).getID() &&
                                    aDate == patInputs[2] && aTime == patInputs[3] && stat != "cancelled") {
                                    throw SlotUnavailableException();
                                }
                            }

                            int pIdx = -1;
                            for (int i = 0; i < pStore.getCount(); i++) {
                                if (pStore.getAt(i).getID() == loggedInUserID) { pIdx = i; break; }
                            }
                            if (pIdx == -1) {
                                bookMsg.setString("Patient record error.");
                                bookMsg.setFillColor(sf::Color::Red);
                                throw InvalidInputException();
                            }

                            Patient p = pStore.getAt(pIdx);
                            float fee = dStore.getAt(dIdx).getFee();

                            if (p.getBalance() < fee) throw InsufficientFundsException();

                            p -= fee;
                            pStore.removeByID(loggedInUserID);
                            pStore.add(p);
                            FileHandler::savePatients(pStore);

                            int newAId = 1;
                            for (int i = 0; i < aStore.getCount(); i++) {
                                if (aStore.getAt(i).getID() >= newAId) newAId = aStore.getAt(i).getID() + 1;
                            }
                            Appointment newA(newAId, loggedInUserID, dStore.getAt(dIdx).getID(),
                                patInputs[2].c_str(), patInputs[3].c_str(), "pending");
                            FileHandler::addAppointment(newA);

                            Storage<Bill> bStore; FileHandler::loadBills(bStore);
                            int newBId = 1;
                            for (int i = 0; i < bStore.getCount(); i++) {
                                if (bStore.getAt(i).getID() >= newBId) newBId = bStore.getAt(i).getID() + 1;
                            }
                            Bill newB(newBId, loggedInUserID, newAId, fee, "unpaid", patInputs[2].c_str());
                            FileHandler::addBill(newB);

                            bookMsg.setString("Success! Appt ID: " + to_string(newAId));
                            bookMsg.setFillColor(sf::Color::Green);
                            patFailAttempts = 0;

                            for (int i = 0; i < 4; i++) {
                                patInputs[i] = "";
                                patDisplays[i].setString("");
                                patBoxes[i].setFillColor(sf::Color(150, 150, 150));
                            }
                            activePatInput = -1;
                        }
                        catch (HospitalException& e) {
                            string current = bookMsg.getString();
                            if (current.empty()) {
                                bookMsg.setString(e.what());
                                bookMsg.setFillColor(sf::Color::Red);
                            }
                        }
                    }
                    else {
                        activePatInput = -1;
                        for (int i = 0; i < 4; i++) {
                            if (patBoxes[i].getGlobalBounds().contains(mousePosF)) {
                                activePatInput = i;
                                patBoxes[i].setFillColor(sf::Color(255, 255, 255));
                                patBoxes[i].setOutlineColor(sf::Color::Yellow);
                            }
                            else {
                                patBoxes[i].setFillColor(sf::Color(150, 150, 150));
                                patBoxes[i].setOutlineColor(sf::Color::Black);
                            }
                        }
                    }
                }
               else if (currentState == STATE_PATIENT_CANCEL_APPOINTMENT || currentState == STATE_PATIENT_PAY_BILL || currentState == STATE_PATIENT_TOP_UP_BALANCE) {

                       if (backBtn.getGlobalBounds().contains(mousePosF)) currentState = STATE_PATIENT_DASHBOARD;
                       else if (singlePatBox.getGlobalBounds().contains(mousePosF)) {
                           activeSinglePatInput = true;
                           singlePatBox.setFillColor(sf::Color(255, 255, 255));
                           singlePatBox.setOutlineColor(sf::Color::Yellow);
                       }
                       else if (singleActionBtn.getGlobalBounds().contains(mousePosF) && !singlePatInput.empty()) {
                           try {
                               Storage<Patient> pStore; FileHandler::loadPatients(pStore);
                               int pIdx = -1;
                               for (int i = 0; i < pStore.getCount(); i++) {
                                   if (pStore.getAt(i).getID() == loggedInUserID) { pIdx = i; break; }
                               }
                               if (pIdx == -1) throw InvalidInputException();

                               Patient p = pStore.getAt(pIdx);

                               for (char c : singlePatInput) {
                                   if (c < '0' || c > '9') throw InvalidInputException();
                               }

                               if (currentState == STATE_PATIENT_TOP_UP_BALANCE) {
                                   float amt = stof(singlePatInput);
                                   if (!Validator::isPositiveFloat(amt)) {
                                       patFailAttempts++;
                                       if (patFailAttempts >= 3) {
                                           currentState = STATE_PATIENT_DASHBOARD;
                                           patFailAttempts = 0;
                                       }
                                       throw InvalidInputException();
                                   }
                                   p += amt;
                                   pStore.removeByID(loggedInUserID);
                                   pStore.add(p);
                                   FileHandler::savePatients(pStore);
                                   singleActionMsg.setString("Balance updated! PKR " + to_string((int)p.getBalance()));
                                   singleActionMsg.setFillColor(sf::Color::Green);
                                   singlePatInput = "";
                                   singlePatDisplay.setString("");
                                   activeSinglePatInput = false;
                                   singlePatBox.setOutlineColor(sf::Color::Black);
                                   patFailAttempts = 0;
                               }
                               else if (currentState == STATE_PATIENT_CANCEL_APPOINTMENT) {
                                   int aID = stoi(singlePatInput);
                                   Storage<Appointment> aStore; FileHandler::loadAppointments(aStore);
                                   Storage<Doctor> dStore; FileHandler::loadDoctors(dStore);
                                   Storage<Bill> bStore; FileHandler::loadBills(bStore);

                                   int aIdx = -1;
                                   for (int i = 0; i < aStore.getCount(); i++) {
                                       if (aStore.getAt(i).getID() == aID &&
                                           aStore.getAt(i).getPatientID() == loggedInUserID) {
                                           aIdx = i; break;
                                       }
                                   }
                                   if (aIdx == -1) {
                                       singleActionMsg.setString("Invalid appointment ID.");
                                       singleActionMsg.setFillColor(sf::Color::Red);
                                       throw InvalidInputException();
                                   }

                                   Appointment a = aStore.getAt(aIdx);
                                   string stat = string(a.getStatus());
                                   while (!stat.empty() && (isspace(stat.back()) || stat.back() == '\r')) stat.pop_back();

                                   if (stat != "pending") {
                                       singleActionMsg.setString("Appointment is not pending.");
                                       singleActionMsg.setFillColor(sf::Color::Red);
                                       throw InvalidInputException();
                                   }

                                   float fee = 0;
                                   for (int i = 0; i < dStore.getCount(); i++) {
                                       if (dStore.getAt(i).getID() == a.getDoctorID()) {
                                           fee = dStore.getAt(i).getFee(); break;
                                       }
                                   }

                                   p += fee;
                                   pStore.removeByID(loggedInUserID);
                                   pStore.add(p);
                                   FileHandler::savePatients(pStore);

                                   aStore.removeByID(aID);
                                   aStore.add(Appointment(aID, loggedInUserID, a.getDoctorID(),
                                       a.getDate(), a.getTimeSlot(), "cancelled"));
                                   FileHandler::saveAppointments(aStore);

                                   for (int i = 0; i < bStore.getCount(); i++) {
                                       if (bStore.getAt(i).getAppointmentID() == aID) {
                                           Bill b = bStore.getAt(i);
                                           bStore.removeByID(b.getID());
                                           bStore.add(Bill(b.getID(), loggedInUserID, aID,
                                               b.getAmount(), "cancelled", b.getDate()));
                                           FileHandler::saveBills(bStore);
                                           break;
                                       }
                                   }
                                   singleActionMsg.setString("Appt cancelled. Refunded: PKR " + to_string((int)fee));
                                   singleActionMsg.setFillColor(sf::Color::Green);
                                   singlePatInput = "";
                                   singlePatDisplay.setString("");
                                   activeSinglePatInput = false;
                                   singlePatBox.setOutlineColor(sf::Color::Black);
                               }
                               else if (currentState == STATE_PATIENT_PAY_BILL) {
                                   int bID = stoi(singlePatInput);
                                   Storage<Bill> bStore; FileHandler::loadBills(bStore);

                                   int bIdx = -1;
                                   for (int i = 0; i < bStore.getCount(); i++) {
                                       if (bStore.getAt(i).getID() == bID &&
                                           bStore.getAt(i).getPatientID() == loggedInUserID) {
                                           bIdx = i; break;
                                       }
                                   }
                                   if (bIdx == -1) {
                                       singleActionMsg.setString("Invalid bill ID.");
                                       singleActionMsg.setFillColor(sf::Color::Red);
                                       throw InvalidInputException();
                                   }

                                   Bill b = bStore.getAt(bIdx);
                                   string stat = string(b.getStatus());
                                   while (!stat.empty() && (isspace(stat.back()) || stat.back() == '\r')) stat.pop_back();

                                   if (stat != "unpaid") {
                                       singleActionMsg.setString("Bill is not unpaid.");
                                       singleActionMsg.setFillColor(sf::Color::Red);
                                       throw InvalidInputException();
                                   }

                                   if (p.getBalance() < b.getAmount()) throw InsufficientFundsException();

                                   p -= b.getAmount();
                                   pStore.removeByID(loggedInUserID);
                                   pStore.add(p);
                                   FileHandler::savePatients(pStore);

                                   bStore.removeByID(bID);
                                   bStore.add(Bill(bID, loggedInUserID, b.getAppointmentID(),
                                       b.getAmount(), "paid", b.getDate()));
                                   FileHandler::saveBills(bStore);

                                   singleActionMsg.setString("Bill paid! Remaining: PKR " + to_string((int)p.getBalance()));
                                   singleActionMsg.setFillColor(sf::Color::Green);
                                   singlePatInput = "";
                                   singlePatDisplay.setString("");
                                   activeSinglePatInput = false;
                                   singlePatBox.setOutlineColor(sf::Color::Black);
                               }
                           }
                           catch (HospitalException& e) {
                               string current = singleActionMsg.getString();
                               if (current.empty()) {
                                   singleActionMsg.setString(e.what());
                                   singleActionMsg.setFillColor(sf::Color::Red);
                               }
                           }
                       }
                       else {
                           activeSinglePatInput = false;
                           singlePatBox.setFillColor(sf::Color(150, 150, 150));
                           singlePatBox.setOutlineColor(sf::Color::Black);
                       }
               }

                else if (currentState == STATE_PATIENT_VIEW_APPOINTMENTS) {
                    bool changed = false;
                    if (backBtn.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_PATIENT_DASHBOARD;
                    }
                    else if (nextBtn.getGlobalBounds().contains(mousePosF)) {
                        currentApptPage++; changed = true;
                    }
                    else if (prevBtn.getGlobalBounds().contains(mousePosF) && currentApptPage > 0) {
                        currentApptPage--; changed = true;
                    }
                    if (changed) {
                        refreshPatAppointments(patViewApptCol1, patViewApptCol2, patViewApptCol3,
                            patViewApptCol4, patViewApptCol5, patViewApptCol6,
                            loggedInUserID, currentApptPage);
                    }
                }
                else if (currentState == STATE_PATIENT_VIEW_RECORDS) {
                    bool changed = false;
                    if (backBtn.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_PATIENT_DASHBOARD;
                    }
                    else if (nextBtn.getGlobalBounds().contains(mousePosF)) {
                        currentDocPage++; changed = true;
                    }
                    else if (prevBtn.getGlobalBounds().contains(mousePosF) && currentDocPage > 0) {
                        currentDocPage--; changed = true;
                    }
                    if (changed) {
                        refreshPatRecords(patViewRecCol1, patViewRecCol2, patViewRecCol3,
                            patViewRecCol4, loggedInUserID, currentDocPage);
                    }
                }
                else if (currentState == STATE_PATIENT_VIEW_BILLS) {
                    bool changed = false;
                    if (backBtn.getGlobalBounds().contains(mousePosF)) {
                        currentState = STATE_PATIENT_DASHBOARD;
                    }
                    else if (nextBtn.getGlobalBounds().contains(mousePosF)) {
                        currentBillPage++; changed = true;
                    }
                    else if (prevBtn.getGlobalBounds().contains(mousePosF) && currentBillPage > 0) {
                        currentBillPage--; changed = true;
                    }
                    if (changed) {
                        refreshPatBills(patViewBillCol1, patViewBillCol2, patViewBillCol3,
                            patViewBillCol4, patViewBillCol5,
                            loggedInUserID, currentBillPage, patTotalBillsMsg);
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                if (currentState == STATE_ADMIN_LOGIN || currentState == STATE_PATIENT_LOGIN || currentState == STATE_DOCTOR_LOGIN) {
                    bool isSuccess = false;
                    int numericId = atoi(idBuffer);
                    LogicSystem attempt(numericId, passBuffer);

                    if (currentState == STATE_ADMIN_LOGIN) {
                        isSuccess = attempt.isExist("admin.txt", "Admin");
                    }
                    else if (currentState == STATE_DOCTOR_LOGIN) {
                        isSuccess = attempt.isExist("doctors.txt", "Doctor");
                    }
                    else if (currentState == STATE_PATIENT_LOGIN) {
                        isSuccess = attempt.isExist("patients.txt", "Patient");
                    }

                    if (isSuccess) {
                        showError = false;
                        loggedInUserID = numericId;

                        if (currentState == STATE_ADMIN_LOGIN) {
                            loggedInUserName = "Admin";
                            currentState = STATE_ADMIN_DASHBOARD;
                        }
                        else if (currentState == STATE_DOCTOR_LOGIN) {
                            Storage<Doctor> dStore; FileHandler::loadDoctors(dStore);
                            loggedInUserName = "Doctor";
                            for (int i = 0; i < dStore.getCount(); i++) {
                                if (dStore.getAt(i).getID() == loggedInUserID) {
                                    char buf[50]; int k = 0;
                                    while (dStore.getAt(i).getName()[k] != '\0' && k < 49) { buf[k] = dStore.getAt(i).getName()[k]; k++; }
                                    buf[k] = '\0'; manualTrim(buf);
                                    loggedInUserName = string(buf);
                                    break;
                                }
                            }
                            currentState = STATE_DOCTOR_DASHBOARD;
                        }
                        else if (currentState == STATE_PATIENT_LOGIN) {
                            Storage<Patient> pStore; FileHandler::loadPatients(pStore);
                            loggedInUserName = "Patient";
                            for (int i = 0; i < pStore.getCount(); i++) {
                                if (pStore.getAt(i).getID() == loggedInUserID) {
                                    char buf[50]; int k = 0;
                                    while (pStore.getAt(i).getName()[k] != '\0' && k < 49) { buf[k] = pStore.getAt(i).getName()[k]; k++; }
                                    buf[k] = '\0'; manualTrim(buf);
                                    loggedInUserName = string(buf);
                                    break;
                                }
                            }
                            currentState = STATE_PATIENT_DASHBOARD;
                        }
                        welcomeText.setString("Welcome, " + loggedInUserName);
                    }
                    else {
                        attempt.failedAttempt();
                        showError = true;

                        if (!attempt.canCheckAgain()) {
                            showError = false;
                            cout << "LOCKED OUT! Sending back to Home Screen." << endl;
                            currentState = STATE_LOCK_STATE;
                            lockoutTimer.restart();
                        }

                        idBuffer[0] = '\0';
                        idCharCount = 0;
                        passBuffer[0] = '\0';
                        passCharCount = 0;

                        isIdActive = true;
                        isPassActive = false;
                    }
                }
            }
        } 

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (currentState == STATE_HOME) {
            if (bgTextBox.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                bgTextBox.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                bgTextBox.setFillColor(sf::Color(255, 215, 0));
            }
        }

        if (currentState == STATE_LOGIN_SELECTION) {
            if (loginTextBox1.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                loginTextBox1.setFillColor(sf::Color(100, 100, 255));
            }
            else {
                loginTextBox1.setFillColor(sf::Color(60, 60, 255));
            }
            if (loginTextBox2.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                loginTextBox2.setFillColor(sf::Color(100, 100, 255));
            }
            else {
                loginTextBox2.setFillColor(sf::Color(60, 60, 255));
            }
            if (loginTextBox3.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                loginTextBox3.setFillColor(sf::Color(100, 100, 255));
            }
            else {
                loginTextBox3.setFillColor(sf::Color(60, 60, 255));
            }
        }
        if (currentState == STATE_ADMIN_LOGIN || currentState == STATE_PATIENT_LOGIN || currentState == STATE_DOCTOR_LOGIN) {
            if (portalTextBox1.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                portalTextBox1.setFillColor(sf::Color(0, 0, 0, 128));
            }
            else {
                portalTextBox1.setFillColor(sf::Color(150, 150, 150));
            }
            if (portalTextBox2.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                portalTextBox2.setFillColor(sf::Color(0, 0, 0, 128));
            }
            else {
                portalTextBox2.setFillColor(sf::Color(150, 150, 150));
            }
        }
        if (currentState == STATE_ADMIN_DASHBOARD) {
            if (adminTextBox1.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                adminTextBox1.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                adminTextBox1.setFillColor(sf::Color(255, 215, 0));
            }
            if (adminTextBox2.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                adminTextBox2.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                adminTextBox2.setFillColor(sf::Color(255, 215, 0));
            }
            if (adminTextBox3.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                adminTextBox3.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                adminTextBox3.setFillColor(sf::Color(255, 215, 0));
            }
            if (adminTextBox4.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                adminTextBox4.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                adminTextBox4.setFillColor(sf::Color(255, 215, 0));
            }
            if (adminTextBox5.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                adminTextBox5.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                adminTextBox5.setFillColor(sf::Color(255, 215, 0));
            }
            if (adminTextBox6.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                adminTextBox6.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                adminTextBox6.setFillColor(sf::Color(255, 215, 0));
            }
            if (adminTextBox7.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                adminTextBox7.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                adminTextBox7.setFillColor(sf::Color(255, 215, 0));
            }
            if (adminTextBox8.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                adminTextBox8.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                adminTextBox8.setFillColor(sf::Color(255, 215, 0));
            }
            if (adminTextBox9.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                adminTextBox9.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                adminTextBox9.setFillColor(sf::Color(255, 215, 0));
            }
            if (adminTextBox10.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                adminTextBox10.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                adminTextBox10.setFillColor(sf::Color(255, 215, 0));
            }
        }

        if (currentState == STATE_DOCTOR_DASHBOARD) {
            if (doctorTextBox1.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                doctorTextBox1.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                doctorTextBox1.setFillColor(sf::Color(255, 215, 0));
            }

            if (doctorTextBox2.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                doctorTextBox2.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                doctorTextBox2.setFillColor(sf::Color(255, 215, 0));
            }

            if (doctorTextBox3.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                doctorTextBox3.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                doctorTextBox3.setFillColor(sf::Color(255, 215, 0));
            }

            if (doctorTextBox4.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                doctorTextBox4.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                doctorTextBox4.setFillColor(sf::Color(255, 215, 0));
            }

            if (doctorTextBox5.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                doctorTextBox5.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                doctorTextBox5.setFillColor(sf::Color(255, 215, 0));
            }

            if (doctorTextBox6.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                doctorTextBox6.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                doctorTextBox6.setFillColor(sf::Color(255, 215, 0));
            }
        }
        if (currentState == STATE_PATIENT_DASHBOARD) {
            if (patientTextBox1.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                patientTextBox1.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                patientTextBox1.setFillColor(sf::Color(255, 215, 0));
            }

            if (patientTextBox2.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                patientTextBox2.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                patientTextBox2.setFillColor(sf::Color(255, 215, 0));
            }

            if (patientTextBox3.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                patientTextBox3.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                patientTextBox3.setFillColor(sf::Color(255, 215, 0));
            }

            if (patientTextBox4.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                patientTextBox4.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                patientTextBox4.setFillColor(sf::Color(255, 215, 0));
            }

            if (patientTextBox5.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                patientTextBox5.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                patientTextBox5.setFillColor(sf::Color(255, 215, 0));
            }

            if (patientTextBox6.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                patientTextBox6.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                patientTextBox6.setFillColor(sf::Color(255, 215, 0));
            }

            if (patientTextBox7.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                patientTextBox7.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                patientTextBox7.setFillColor(sf::Color(255, 215, 0));
            }

            if (patientTextBox8.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                patientTextBox8.setFillColor(sf::Color(255, 255, 100));
            }
            else {
                patientTextBox8.setFillColor(sf::Color(255, 215, 0));
            }
        }
        if (currentState == STATE_LOCK_STATE) {
            int timePassed = static_cast<int>(lockoutTimer.getElapsedTime().asSeconds());
            int timeRemaining = 8 - timePassed;

            if (timeRemaining <= 0) {
                currentState = STATE_HOME;
                LogicSystem::tries = 0;
            }
            else {
                countdownText.setString("Please wait " + to_string(timeRemaining) + " seconds...");
                sf::FloatRect newBounds = countdownText.getLocalBounds();
                countdownText.setOrigin(newBounds.width / 2.0f, newBounds.height / 2.0f);
            }
        }
        displayLine2.setString(idBuffer);
        char mask[32] = "";
        for (int i = 0; i < passCharCount; i++) mask[i] = '*';
        mask[passCharCount] = '\0';
        displayLine1.setString(mask);

        window.clear(sf::Color::Black);

        if (currentState == STATE_HOME) {
            window.draw(BG);
            window.draw(bgLogo);
            window.draw(bgTextBox);
            window.draw(backgroundText);
        }
        else if (currentState == STATE_LOGIN_SELECTION) {
            window.draw(loginPg);
            window.draw(loginTextBox1);
            window.draw(loginTextBox2);
            window.draw(loginTextBox3);
            window.draw(loginText1);
            window.draw(loginText2);
            window.draw(loginText3);
        }
        else if (currentState == STATE_ADMIN_LOGIN) {
            window.draw(loginPortal);
            window.draw(portalBackBtn);
            window.draw(portalBackTxt);
            window.draw(portalTextBox1);
            window.draw(portalTextBox2);
            window.draw(id);
            window.draw(password);
            window.draw(displayLine1);
            window.draw(displayLine2);
        }
        else if (currentState == STATE_DOCTOR_LOGIN) {
            window.draw(loginPortal);
            window.draw(portalBackBtn);
            window.draw(portalBackTxt);
            window.draw(portalTextBox1);
            window.draw(portalTextBox2);
            window.draw(id);
            window.draw(password);
            window.draw(displayLine1);
            window.draw(displayLine2);
        }
        else if (currentState == STATE_PATIENT_LOGIN) {
            window.draw(loginPortal);
            window.draw(portalBackBtn);
            window.draw(portalBackTxt);
            window.draw(portalTextBox1);
            window.draw(portalTextBox2);
            window.draw(id);
            window.draw(password);
            window.draw(displayLine1);
            window.draw(displayLine2);
        }
        else if (currentState == STATE_ADMIN_DASHBOARD) {
            window.draw(adminPortal);
            welcomeText.setPosition(390.f, 30.f);
            window.draw(welcomeText);
            window.draw(adminTextBox1);
            window.draw(adminTextBox2);
            window.draw(adminTextBox3);
            window.draw(adminTextBox4);
            window.draw(adminTextBox5);
            window.draw(adminTextBox6);
            window.draw(adminTextBox7);
            window.draw(adminTextBox8);
            window.draw(adminTextBox9);
            window.draw(adminTextBox10);
            window.draw(adminText1);
            window.draw(adminText2);
            window.draw(adminText3);
            window.draw(adminText4);
            window.draw(adminText5);
            window.draw(adminText6);
            window.draw(adminText7);
            window.draw(adminText8);
            window.draw(adminText9);
            window.draw(adminText10);
        }
        else if (currentState == STATE_PATIENT_DASHBOARD) {
            window.draw(patientPortal);
            welcomeText.setPosition(290.f, 30.f);
            window.draw(welcomeText);
            window.draw(patientTextBox1);
            window.draw(patientTextBox2);
            window.draw(patientTextBox3);
            window.draw(patientTextBox4);
            window.draw(patientTextBox5);
            window.draw(patientTextBox6);
            window.draw(patientTextBox7);
            window.draw(patientTextBox8);
            window.draw(patientText1);
            window.draw(patientText2);
            window.draw(patientText3);
            window.draw(patientText4);
            window.draw(patientText5);
            window.draw(patientText6);
            window.draw(patientText7);
            window.draw(patientText8);
        }
        else if (currentState == STATE_DOCTOR_DASHBOARD) {
            window.draw(doctorPortal);
            welcomeText.setPosition(270.f, 30.f);
            window.draw(welcomeText);
            window.draw(doctorTextBox1);
            window.draw(doctorTextBox2);
            window.draw(doctorTextBox3);
            window.draw(doctorTextBox4);
            window.draw(doctorTextBox5);
            window.draw(doctorTextBox6);
            window.draw(doctorText1);
            window.draw(doctorText2);
            window.draw(doctorText3);
            window.draw(doctorText4);
            window.draw(doctorText5);
            window.draw(doctorText6);
        }
        else if (currentState == STATE_LOCK_STATE) {
            window.draw(BG);
            window.draw(lockedText);
            window.draw(countdownText);
        }
        else if (currentState == STATE_ADMIN_ADD_DOCTOR) {
            window.draw(textPage);
            for (int i = 0; i < 6; i++) {
                window.draw(docBoxes[i]);
                window.draw(docLabels[i]);
                window.draw(docDisplay[i]);
            }
            window.draw(backBtn);
            window.draw(navBackTxt);
            window.draw(saveBtn);
            window.draw(saveTxt);
            if (showAddDocError) {
                window.draw(addDocErrorMsg);
            }
        }
        else if (currentState == STATE_ADMIN_REMOVE_DOCTOR) {
                window.draw(textPage);
                window.draw(removeDocBox);
                window.draw(removeDocLabel);
                window.draw(removeDocDisplay);
                window.draw(confirmRemoveBtn);
                window.draw(confirmRemoveTxt);
                window.draw(backBtn);
                window.draw(navBackTxt);
                window.draw(removeDocStatusMsg);
        }
        else if (currentState == STATE_ADMIN_VIEW_PATIENTS) {
            window.draw(textPage);
            window.draw(patHeader);
            window.draw(backBtn);

            window.draw(navBackTxt);

            for (int i = 0; i < 10; i++) {
                window.draw(pCol1[i]); window.draw(pCol2[i]);
                window.draw(pCol3[i]); window.draw(pCol4[i]);
                window.draw(pCol5[i]); window.draw(pCol6[i]);
            }
            window.draw(nextBtn);
            window.draw(navNextTxt);

            if (currentPatPage > 0) {
                window.draw(prevBtn);
                window.draw(navPrevTxt);
            }
        }
        else if (currentState == STATE_ADMIN_VIEW_DOCTORS) {
            window.draw(textPage);
            window.draw(viewDocHeader);
            window.draw(backBtn);
            window.draw(navBackTxt);

            for (int i = 0; i < 10; i++) {
                window.draw(colID[i]);
                window.draw(colName[i]);
                window.draw(colSpec[i]);
                window.draw(colFee[i]);
            }

            window.draw(nextBtn);
            window.draw(navNextTxt);

            if (currentDocPage > 0) {
                window.draw(prevBtn);
                window.draw(navPrevTxt);

                window.draw(navBackTxt);
            }
            }
        else if (currentState == STATE_ADMIN_VIEW_APPOINTMENTS) {
            window.draw(textPage);
            window.draw(apptHeader);
            window.draw(backBtn);
            window.draw(navBackTxt);

            for (int i = 0; i < 10; i++) {
                window.draw(aCol1[i]);
                window.draw(aCol2[i]);
                window.draw(aCol3[i]);
                window.draw(aCol4[i]);
                window.draw(aCol5[i]);
                window.draw(aCol6[i]);
            }
            window.draw(nextBtn);
            window.draw(navNextTxt);

            if (currentApptPage > 0) {
                window.draw(prevBtn);
                window.draw(navPrevTxt);
            }
        }
        else if (currentState == STATE_ADMIN_VIEW_BILLS) {
            window.draw(textPage);
            window.draw(billsHeader);

            window.draw(backBtn);
            window.draw(navBackTxt);

            for (int i = 0; i < 10; i++) {
                window.draw(bCol1[i]);
                window.draw(bCol2[i]);
                window.draw(bCol3[i]);
                window.draw(bCol4[i]);
            }
            window.draw(nextBtn);
            window.draw(navNextTxt);

            if (currentBillPage > 0) {
                window.draw(prevBtn);
                window.draw(navPrevTxt);
            }
        }
        else if (currentState == STATE_ADMIN_DISCHARGE_PATIENT) {
            window.draw(textPage);
            window.draw(backBtn);
            window.draw(navBackTxt);

            window.draw(dischargeLabel);
            window.draw(dischargeBox);
            window.draw(dischargeDisplay);
            
            window.draw(confirmDischargeBtn);
            window.draw(confirmDischargeTxt);
            window.draw(dischargeStatusMsg);
        }
        else if (currentState == STATE_ADMIN_VIEW_SECURITY_LOG) {
            window.draw(textPage);
            window.draw(logHeader);
            window.draw(backBtn);
            window.draw(navBackTxt);

            for (int i = 0; i < 12; i++) {
                window.draw(logLines[i]);
            }

            window.draw(nextBtn);
            window.draw(navNextTxt);

            if (currentLogPage > 0) {
                window.draw(prevBtn);
                window.draw(navPrevTxt);
            }
            }
        else if (currentState == STATE_ADMIN_GENERATE_REPORT) {
            window.draw(textPage);
            window.draw(backBtn);
            window.draw(navBackTxt);

            window.draw(reportStats);
            window.draw(reportPatHeader);
            window.draw(reportDocHeader);

            for (int i = 0; i < 5; i++) {
                window.draw(reportPatRows[i]);
                window.draw(reportDocRows[i]);
            }

            window.draw(reportPatNextBtn);
            window.draw(rPatNextTxt);
            if (reportPatPage > 0) {
                window.draw(reportPatPrevBtn);
                window.draw(rPatPrevTxt);
            }
            window.draw(reportDocNextBtn);
            window.draw(rDocNextTxt);
            if (reportDocPage > 0) {
                window.draw(reportDocPrevBtn);
                window.draw(rDocPrevTxt);
            }
        }
        else if (currentState == STATE_DOCTOR_VIEW_APPOINTMENTS) {
            window.draw(textPage); window.draw(backBtn); window.draw(navBackTxt);
            window.draw(daHeader);
            for (int i = 0; i < 10; i++) { window.draw(docApptCol1[i]); window.draw(docApptCol2[i]); window.draw(docApptCol3[i]); window.draw(docApptCol4[i]); }
            window.draw(nextBtn); window.draw(navNextTxt);
            if (docApptPage > 0) { window.draw(prevBtn); window.draw(navPrevTxt); }
        }
        else if (currentState == STATE_DOCTOR_MARK_COMPLETE || currentState == STATE_DOCTOR_MARK_NO_SHOW) {
            docActionBtn.setPosition(350.f, 320.f);
            docActionBtn.setSize(sf::Vector2f(240.f, 60.f));
            window.draw(textPage); window.draw(backBtn); window.draw(navBackTxt);
            window.draw(dActionLabel); window.draw(dActionBox); window.draw(dActionDisplay);
            docActionBtnTxt.setString("CONFIRM");
            docActionBtnTxt.setPosition(390.f, 332.f);
            window.draw(docActionBtn);
            window.draw(docActionBtnTxt);
            dActionMsg.setPosition(300.f, 400.f);
            window.draw(dActionMsg);
        }
        else if (currentState == STATE_DOCTOR_WRITE_PRESCRIPTION) {
            window.draw(textPage);
            window.draw(backBtn);
            window.draw(navBackTxt);
            for (int i = 0; i < 3; i++) {
                window.draw(rxLabels[i]);
                window.draw(rxBoxes[i]);
                window.draw(rxDisplays[i]);
            }
            docActionBtn.setPosition(400.f, 420.f);
            docActionBtnTxt.setString("SAVE");
            docActionBtnTxt.setPosition(450.f, 430.f);
            window.draw(docActionBtn);
            window.draw(docActionBtnTxt);
            dActionMsg.setPosition(300.f, 370.f);
            window.draw(dActionMsg);
        }
        else if (currentState == STATE_DOCTOR_VIEW_HISTORY) {
            window.draw(textPage); window.draw(backBtn); window.draw(navBackTxt);
            window.draw(dActionLabel); window.draw(dActionBox); window.draw(dActionDisplay);

            window.draw(histLoadBtn);
            window.draw(histLoadTxt);
            dActionMsg.setPosition(220.f, 215.f);
            window.draw(dActionMsg);
            if (currentHistPatID != -1) {
                for (int i = 0; i < 4; i++) { window.draw(docHistCol1[i]); window.draw(docHistCol2[i]); window.draw(docHistCol3[i]); }
                window.draw(nextBtn); backTxt.setString("Next >"); backTxt.setPosition(725.f, 525.f); window.draw(backTxt);
                if (histPage > 0) { window.draw(prevBtn); backTxt.setString("< Prev"); backTxt.setPosition(205.f, 525.f); window.draw(backTxt); }
            }
        }
        else if (currentState == STATE_PATIENT_BOOK_APPOINTMENT) {
            window.draw(textPage); window.draw(navBackTxt);
            for (int i = 0; i < 4; i++) { window.draw(patLabels[i]); window.draw(patBoxes[i]); window.draw(patDisplays[i]); }

            window.draw(bookBtn);
            window.draw(bookBtnTxt);

            bookMsg.setPosition(300.f, 470.f);
            window.draw(bookMsg);
            }
        else if (currentState == STATE_PATIENT_CANCEL_APPOINTMENT || currentState == STATE_PATIENT_PAY_BILL || currentState == STATE_PATIENT_TOP_UP_BALANCE) {
                window.draw(textPage); window.draw(backBtn); window.draw(navBackTxt);
                window.draw(singlePatLabel); window.draw(singlePatBox); window.draw(singlePatDisplay);

                if (currentState == STATE_PATIENT_CANCEL_APPOINTMENT) { singleActionBtnTxt.setString("CANCEL"); singleActionBtnTxt.setPosition(555.f, 332.f); }
                else if (currentState == STATE_PATIENT_PAY_BILL) { singleActionBtnTxt.setString("PAY BILL"); singleActionBtnTxt.setPosition(517.f, 332.f); }
                else { singleActionBtnTxt.setString("TOP UP"); singleActionBtnTxt.setPosition(520.f, 332.f); }

                window.draw(singleActionBtn);
                window.draw(singleActionBtnTxt);

                singleActionMsg.setPosition(300.f, 400.f);
                window.draw(singleActionMsg);
        }
        else if (currentState == STATE_PATIENT_VIEW_APPOINTMENTS) {
            window.draw(textPage);
            window.draw(backBtn); window.draw(navBackTxt);
            window.draw(patViewApptHeader);
            for (int i = 0; i < 10; i++) {
                window.draw(patViewApptCol1[i]); window.draw(patViewApptCol2[i]);
                window.draw(patViewApptCol3[i]); window.draw(patViewApptCol4[i]);
                window.draw(patViewApptCol5[i]); window.draw(patViewApptCol6[i]);
            }
            window.draw(nextBtn); window.draw(navNextTxt);
            if (currentApptPage > 0) {
                window.draw(prevBtn); window.draw(navPrevTxt);
            }
        }
        else if (currentState == STATE_PATIENT_VIEW_RECORDS) {
            window.draw(textPage);
            window.draw(backBtn); window.draw(navBackTxt);
            window.draw(patViewRecHeader);
            for (int i = 0; i < 10; i++) {
                window.draw(patViewRecCol1[i]); window.draw(patViewRecCol2[i]);
                window.draw(patViewRecCol3[i]); window.draw(patViewRecCol4[i]);
            }
            window.draw(nextBtn); window.draw(navNextTxt);
            if (currentDocPage > 0) {
                window.draw(prevBtn); window.draw(navPrevTxt);
            }
        }
        else if (currentState == STATE_PATIENT_VIEW_BILLS) {
            window.draw(textPage);
            window.draw(backBtn); window.draw(navBackTxt);
            window.draw(patViewBillHeader);
            for (int i = 0; i < 10; i++) {
                window.draw(patViewBillCol1[i]); window.draw(patViewBillCol2[i]);
                window.draw(patViewBillCol3[i]); window.draw(patViewBillCol4[i]);
                window.draw(patViewBillCol5[i]);
            }
            window.draw(patTotalBillsMsg);
            window.draw(nextBtn); window.draw(navNextTxt);
            if (currentBillPage > 0) {
                window.draw(prevBtn); window.draw(navPrevTxt);
            }
        }
        if (showError) {
            window.draw(errorMsg);
        }
        window.display();
    }
}

void manualTrim(char* str) {
    if (str == nullptr || str[0] == '\0') return;
    int len = 0;
    while (str[len] != '\0') len++;
    int i = len - 1;
    while (i >= 0 && (str[i] == '.' || str[i] == ' ' || (int)str[i] < 32)) {
        str[i] = '\0';
        i--;
    }
}

void refreshDoctorList(sf::Text* colID, sf::Text* colName, sf::Text* colSpec, sf::Text* colFee, int page) {
    Storage<Doctor>* tempStore = new Storage<Doctor>();
    FileHandler::loadDoctors(*tempStore);

    int totalDocs = tempStore->getCount();
    int startIdx = page * 10;

    for (int i = 0; i < 10; i++) {
        int actualIdx = startIdx + i;
        if (actualIdx < totalDocs) {
            const Doctor& d = tempStore->getAt(actualIdx);
            char nBuf[50], sBuf[50];
            int k = 0; while (d.getName()[k] != '\0' && k < 49) { nBuf[k] = d.getName()[k]; k++; } nBuf[k] = '\0';
            k = 0; while (d.getSpecialization()[k] != '\0' && k < 49) { sBuf[k] = d.getSpecialization()[k]; k++; } sBuf[k] = '\0';

            manualTrim(nBuf);
            manualTrim(sBuf);

            colID[i].setString(std::to_string(d.getID()));
            colName[i].setString(nBuf);
            colSpec[i].setString(sBuf);
            colFee[i].setString(std::to_string((int)d.getFee()));
        }
        else {
            colID[i].setString(""); colName[i].setString("");
            colSpec[i].setString(""); colFee[i].setString("");
        }
    }
    delete tempStore;
}

void refreshPatientList(sf::Text* c1, sf::Text* c2, sf::Text* c3, sf::Text* c4, sf::Text* c5, sf::Text* c6, int page) {
    Storage<Patient>* patStore = new Storage<Patient>();
    FileHandler::loadPatients(*patStore);

    Storage<Bill>* billStore = new Storage<Bill>();
    FileHandler::loadBills(*billStore);

    int start = page * 10;
    for (int i = 0; i < 10; i++) {
        int idx = start + i;
        if (idx < patStore->getCount()) {
            const Patient& p = patStore->getAt(idx);
            int unpaidCount = 0;
            for (int j = 0; j < billStore->getCount(); j++) {
                const Bill& b = billStore->getAt(j);
                string currentStatus = b.getStatus();
                if (b.getPatientID() == p.getID() && currentStatus == "unpaid") {
                    unpaidCount++;
                }
            }
            char nBuf[50];
            int k = 0;
            while (p.getName()[k] != '\0' && k < 49) {
                nBuf[k] = p.getName()[k];
                k++;
            }
            nBuf[k] = '\0';
            manualTrim(nBuf);
            float bal = p.getBalance();
            int intPart = (int)bal;
            int fracPart = (int)((bal - intPart) * 100.0f + 0.5f);

            if (fracPart >= 100) {
                intPart += 1;
                fracPart -= 100;
            }
            string balStr = to_string(intPart) + ".";
            if (fracPart < 10) {
                balStr += "0";
            }
            balStr += to_string(fracPart);
            c1[i].setString(to_string(p.getID()));
            c2[i].setString(nBuf);
            c3[i].setString(to_string(p.getAge()) + " " + p.getGender());
            c4[i].setString(p.getContact());
            c5[i].setString(balStr);
            c6[i].setString(to_string(unpaidCount));

        }
        else {
            c1[i].setString(""); c2[i].setString(""); c3[i].setString("");
            c4[i].setString(""); c5[i].setString(""); c6[i].setString("");
        }
    }
    delete patStore;
    delete billStore;
}

void refreshUnpaidBills(sf::Text* c1, sf::Text* c2, sf::Text* c3, sf::Text* c4, int page) {
    Storage<Bill>* billStore = new Storage<Bill>();
    FileHandler::loadBills(*billStore);

    Storage<Patient>* patStore = new Storage<Patient>();
    FileHandler::loadPatients(*patStore);

    int start = page * 10;
    int displayCount = 0;
    int billIdx = 0;
    int unpaidSkipped = 0;

    while (billIdx < billStore->getCount() && displayCount < 10) {
        const Bill& b = billStore->getAt(billIdx);

        string status = b.getStatus();

        if (status == "unpaid") {
            if (unpaidSkipped < start) {
                unpaidSkipped++;
            }
            else {
                c1[displayCount].setString(to_string(b.getID()));

                char pNameBuf[50] = "Unknown";
                for (int p = 0; p < patStore->getCount(); p++) {
                    if (patStore->getAt(p).getID() == b.getPatientID()) {
                        int k = 0;
                        while (patStore->getAt(p).getName()[k] != '\0' && k < 49) {
                            pNameBuf[k] = patStore->getAt(p).getName()[k];
                            k++;
                        }
                        pNameBuf[k] = '\0';
                        break;
                    }
                }
                manualTrim(pNameBuf);
                c2[displayCount].setString(pNameBuf);
                c3[displayCount].setString(to_string((int)b.getAmount()));
                c4[displayCount].setString(b.getDate());
                c4[displayCount].setFillColor(sf::Color::Black);

                displayCount++;
            }
        }
        billIdx++;
    }
    for (int i = displayCount; i < 10; i++) {
        c1[i].setString(""); c2[i].setString("");
        c3[i].setString(""); c4[i].setString("");
    }

    delete patStore;
    delete billStore;
}

void refreshSecurityLog(sf::Text* lines, int page) {
    ifstream file("security_log.txt");
    if (!file.is_open()) {
        lines[0].setString("No security events logged.");
        for (int i = 1; i < 12; i++) lines[i].setString("");
        return;
    }

    int startLine = page * 12;
    int currentLine = 0;
    string temp;
    while (currentLine < startLine && getline(file, temp)) {
        currentLine++;
    }
    bool hasData = false;
    for (int i = 0; i < 12; i++) {
        if (getline(file, temp)) {
            lines[i].setString(temp);
            hasData = true;
        }
        else {
            lines[i].setString("");
        }
    }
    if (!hasData && page == 0) {
        lines[0].setString("No security events logged.");
    }
    file.close();
}

void refreshAppointments(sf::Text* c1, sf::Text* c2, sf::Text* c3, sf::Text* c4, sf::Text* c5, sf::Text* c6, int page) {
    LogicSystem::sortAppointments();

    Storage<Appointment>* appStore = new Storage<Appointment>();
    FileHandler::loadAppointments(*appStore);

    Storage<Patient>* patStore = new Storage<Patient>();
    FileHandler::loadPatients(*patStore);

    Storage<Doctor>* docStore = new Storage<Doctor>();
    FileHandler::loadDoctors(*docStore);

    int total = appStore->getCount();
    int start = page * 10;

    for (int i = 0; i < 10; i++) {
        int idx = (total - 1) - (start + i);

        if (idx >= 0 && idx < total) {
            const Appointment& a = appStore->getAt(idx);

            char pNameBuf[50] = "Unknown Patient";
            for (int p = 0; p < patStore->getCount(); p++) {
                if (patStore->getAt(p).getID() == a.getPatientID()) {
                    int k = 0;
                    while (patStore->getAt(p).getName()[k] != '\0' && k < 49) {
                        pNameBuf[k] = patStore->getAt(p).getName()[k];
                        k++;
                    }
                    pNameBuf[k] = '\0';
                    break;
                }
            }
            manualTrim(pNameBuf);

            char dNameBuf[50] = "Unknown Doctor";
            for (int d = 0; d < docStore->getCount(); d++) {
                if (docStore->getAt(d).getID() == a.getDoctorID()) {
                    int k = 0;
                    while (docStore->getAt(d).getName()[k] != '\0' && k < 49) {
                        dNameBuf[k] = docStore->getAt(d).getName()[k];
                        k++;
                    }
                    dNameBuf[k] = '\0';
                    break;
                }
            }
            manualTrim(dNameBuf);

            c1[i].setString(to_string(a.getID()));
            c2[i].setString(pNameBuf);
            c3[i].setString(dNameBuf);
            c4[i].setString(a.getDate());
            c5[i].setString(a.getTimeSlot());
            c6[i].setString(a.getStatus());
        }
        else {
            c1[i].setString(""); c2[i].setString(""); c3[i].setString("");
            c4[i].setString(""); c5[i].setString(""); c6[i].setString("");
        }
    }
    delete appStore;
    delete patStore;
    delete docStore;
}

void refreshDoctorAppointments(sf::Text* c1, sf::Text* c2, sf::Text* c3, sf::Text* c4, int docID, int page) {
    Storage<Appointment> appStore; FileHandler::loadAppointments(appStore);
    Storage<Patient> patStore; FileHandler::loadPatients(patStore);
    string today = getTodayStr();

    Appointment* todayAppts = new Appointment[100];
    int count = 0;

    for (int i = 0; i < appStore.getCount(); i++) {
        string appDate = string(appStore.getAt(i).getDate());
        while (!appDate.empty() && isspace(appDate.back())) appDate.pop_back();

        if (appStore.getAt(i).getDoctorID() == docID && appDate == today) {
            todayAppts[count++] = appStore.getAt(i);
        }
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (string(todayAppts[j].getTimeSlot()) > string(todayAppts[j + 1].getTimeSlot())) {
                swap(todayAppts[j], todayAppts[j + 1]);
            }
        }
    }

    int start = page * 10;
    for (int i = 0; i < 10; i++) {
        int idx = start + i;
        if (idx < count) {
            c1[i].setString(to_string(todayAppts[idx].getID()));
            char pName[50] = "Unknown";
            for (int p = 0; p < patStore.getCount(); p++) {
                if (patStore.getAt(p).getID() == todayAppts[idx].getPatientID()) {
                    int k = 0; while (patStore.getAt(p).getName()[k] != '\0' && k < 49) { pName[k] = patStore.getAt(p).getName()[k]; k++; }
                    pName[k] = '\0';
                    break;
                }
            }
            manualTrim(pName);
            c2[i].setString(pName);
            c3[i].setString(todayAppts[idx].getTimeSlot());
            c4[i].setString(todayAppts[idx].getStatus());
        }
        else {
            c1[i].setString(i == 0 && count == 0 ? "No appointments scheduled for today." : "");
            c2[i].setString(""); c3[i].setString(""); c4[i].setString("");
        }
    }
    delete[] todayAppts;
}

bool processDoctorAction(int docID, int apptID, string action, sf::Text& msg) {
    Storage<Appointment> appStore; FileHandler::loadAppointments(appStore);
    Storage<Bill> billStore; FileHandler::loadBills(billStore);
    string today = getTodayStr();

    int aIdx = -1;
    for (int i = 0; i < appStore.getCount(); i++) {
        if (appStore.getAt(i).getID() == apptID) { aIdx = i; break; }
    }

    if (aIdx == -1) { msg.setString("Error: Appt ID not found."); msg.setFillColor(sf::Color::Red); return false; }
    Appointment a = appStore.getAt(aIdx);

    string appDate = string(a.getDate());
    while (!appDate.empty() && isspace(appDate.back())) appDate.pop_back();
    string stat = string(a.getStatus());
    while (!stat.empty() && isspace(stat.back())) stat.pop_back();

    if (a.getDoctorID() != docID) { msg.setString("Error: Appt belongs to another doctor."); msg.setFillColor(sf::Color::Red); return false; }
    if (stat != "pending" && stat != "scheduled") { msg.setString("Error: Appt is not pending\n or scheduled."); msg.setFillColor(sf::Color::Red); return false; }
    if (appDate != today) { msg.setString("Error: Appt is not scheduled for today."); msg.setFillColor(sf::Color::Red); return false; }

    appStore.removeByID(apptID);
    appStore.add(Appointment(apptID, a.getPatientID(), a.getDoctorID(), a.getDate(), a.getTimeSlot(), action.c_str()));
    FileHandler::saveAppointments(appStore);

    if (action == "no-show") {
        int bIdx = -1;
        for (int i = 0; i < billStore.getCount(); i++) {
            if (billStore.getAt(i).getAppointmentID() == apptID) { bIdx = i; break; }
        }
        if (bIdx != -1) {
            Bill b = billStore.getAt(bIdx);
            billStore.removeByID(b.getID());
            billStore.add(Bill(b.getID(), b.getPatientID(), b.getAppointmentID(), b.getAmount(), "cancelled", b.getDate()));
            FileHandler::saveBills(billStore);
        }
    }

    msg.setString("Success: Appt marked as " + action);
    msg.setFillColor(sf::Color::Green);
    return true;
}

bool submitPrescription(int docID, int apptID, string meds, string notes, sf::Text& msg) {
    Storage<Appointment> appStore; FileHandler::loadAppointments(appStore);
    Storage<Prescription> preStore; FileHandler::loadPrescription(preStore);
    int aIdx = -1;
    for (int i = 0; i < appStore.getCount(); i++) {
        if (appStore.getAt(i).getID() == apptID) { aIdx = i; break; }
    }
    if (aIdx == -1) { msg.setString("Error: Appt ID not found."); return false; }
    Appointment a = appStore.getAt(aIdx);

    string stat = string(a.getStatus());
    while (!stat.empty() && isspace(stat.back())) stat.pop_back();

    if (a.getDoctorID() != docID) { msg.setString("Error: Unauthorized."); return false; }
    if (stat != "completed") { msg.setString("Error: Appt not completed."); return false; }

    for (int i = 0; i < preStore.getCount(); i++) {
        if (preStore.getAt(i).getAppointmentID() == apptID) {
            msg.setString("Error: Prescription already\n written for this appt."); msg.setFillColor(sf::Color::Red); return false;
        }
    }

    int newId = 1;
    for (int i = 0; i < preStore.getCount(); i++) {
        if (preStore.getAt(i).getID() >= newId) newId = preStore.getAt(i).getID() + 1;
    }
    Prescription p(newId, apptID, a.getPatientID(), docID, getTodayStr().c_str(), meds.c_str(), notes.c_str());
    FileHandler::addPrescription(p);
    msg.setString("Success: Prescription saved.");
    msg.setFillColor(sf::Color::Green);
    return true;
}

void refreshHistory(sf::Text* c1, sf::Text* c2, sf::Text* c3, int docID, int patID, int page, sf::Text& msg) {
    Storage<Appointment> appStore; FileHandler::loadAppointments(appStore);
    Storage<Prescription> preStore; FileHandler::loadPrescription(preStore);

    bool hasAppt = false;
    for (int i = 0; i < appStore.getCount(); i++) {
        string stat = string(appStore.getAt(i).getStatus());
        while (!stat.empty() && isspace(stat.back())) stat.pop_back();

        if (appStore.getAt(i).getPatientID() == patID && appStore.getAt(i).getDoctorID() == docID && stat == "completed") {
            hasAppt = true; break;
        }
    }
    if (!hasAppt) {
        msg.setString("Access denied.");
        msg.setFillColor(sf::Color::Red);
        for (int i = 0; i < 4; i++) { c1[i].setString(""); c2[i].setString(""); c3[i].setString(""); }
        return;
    }

    msg.setString("Patient records loaded.");
    msg.setFillColor(sf::Color::Green);

    Prescription* px = new Prescription[100];
    int count = 0;
    for (int i = 0; i < preStore.getCount(); i++) {
        if (preStore.getAt(i).getPatientID() == patID && preStore.getAt(i).getDoctorID() == docID) {
            px[count++] = preStore.getAt(i);
        }
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            const char* d1 = px[j].getDate(); const char* d2 = px[j + 1].getDate();
            int v1 = (d1[6] - '0') * 10000 + (d1[3] - '0') * 100 + (d1[0] - '0');
            int v2 = (d2[6] - '0') * 10000 + (d2[3] - '0') * 100 + (d2[0] - '0');
            if (v1 < v2) swap(px[j], px[j + 1]);
        }
    }

    int start = page * 4;
    for (int i = 0; i < 4; i++) {
        int idx = start + i;
        if (idx < count) {
            c1[i].setString(px[idx].getDate());
            c2[i].setString(px[idx].getMedicines());
            c3[i].setString(px[idx].getNotes());
        }
        else {
            c1[i].setString(i == 0 && count == 0 ? "No history found." : "");
            c2[i].setString(""); c3[i].setString("");
        }
    }
    delete[] px;
}

void refreshDailyReport(sf::Text& statsTxt, sf::Text* patientRows, sf::Text* doctorRows, int patPage, int docPage) {

     time_t t = time(nullptr);
     tm now{};
     localtime_s(&now, &t);
     char today[11];
     sprintf_s(today, sizeof(today), "%02d-%02d-%04d", now.tm_mday, now.tm_mon + 1, now.tm_year + 1900);
     string todayStr(today);

    Storage<Appointment> appStore; FileHandler::loadAppointments(appStore);
    Storage<Bill>        billStore; FileHandler::loadBills(billStore);
    Storage<Patient>     patStore;  FileHandler::loadPatients(patStore);
    Storage<Doctor>      docStore;  FileHandler::loadDoctors(docStore);

    int total = 0, pending = 0, completed = 0, noShow = 0, cancelled = 0;
    float revenue = 0.0f;

    for (int i = 0; i < appStore.getCount(); i++) {
        string appDate = string(appStore.getAt(i).getDate());
        while (!appDate.empty() && (appDate.back() == ' ' || appDate.back() == '\r' || appDate.back() == '\n'))
            appDate.pop_back();

        if (appDate == todayStr) {
            total++;
            string stat = string(appStore.getAt(i).getStatus());
            if (stat == "pending" || stat == "scheduled") pending++;
            else if (stat == "completed") completed++;
            else if (stat == "no-show")    noShow++;
            else if (stat == "cancelled") cancelled++;
        }
    }

    for (int i = 0; i < billStore.getCount(); i++) {
        string billDate = string(billStore.getAt(i).getDate());
        while (!billDate.empty() && (billDate.back() == ' ' || billDate.back() == '\r' || billDate.back() == '\n'))
            billDate.pop_back();
        if (billDate == todayStr && string(billStore.getAt(i).getStatus()) == "paid")
            revenue += billStore.getAt(i).getAmount();
    }

    string mainStats =
        "Date: " + todayStr +
        "\nTotal Appointments Today: " + to_string(total) +
        "\nPending: " + to_string(pending) +
        "  Completed: " + to_string(completed) +
        "  No-show: " + to_string(noShow) +
        "  Cancelled: " + to_string(cancelled) +
        "\nRevenue Collected Today: PKR " + to_string((int)revenue);

    statsTxt.setString(mainStats);
    statsTxt.setFillColor(sf::Color::Black);
    string patRowStrings[100];
    int totalPatRows = 0;

    for (int i = 0; i < patStore.getCount(); i++) {
        float totalOwed = 0.0f;
        int pid = patStore.getAt(i).getID();

        for (int j = 0; j < billStore.getCount(); j++) {
            if (billStore.getAt(j).getPatientID() == pid && string(billStore.getAt(j).getStatus()) == "unpaid") {
                totalOwed += billStore.getAt(j).getAmount();
            }
        }
        if (totalOwed > 0.0f) {
            char nBuf[50];
            int k = 0;
            while (patStore.getAt(i).getName()[k] != '\0' && k < 49) { nBuf[k] = patStore.getAt(i).getName()[k]; k++; }
            nBuf[k] = '\0';
            manualTrim(nBuf);

            patRowStrings[totalPatRows] = string(nBuf) + "  |  PKR " + to_string((int)totalOwed);
            totalPatRows++;
        }
    }
    int patStart = patPage * 5;
    for (int i = 0; i < 5; i++) {
        int idx = patStart + i;
        if (idx < totalPatRows) {
            patientRows[i].setString(patRowStrings[idx]);
            patientRows[i].setFillColor(sf::Color(139, 0, 0));
        }
        else {
            patientRows[i].setString(i == 0 && totalPatRows == 0 ? "  None" : "");
        }
    }
    string docRowStrings[100];
    int totalDocRows = 0;

    for (int i = 0; i < docStore.getCount(); i++) {
        int did = docStore.getAt(i).getID();
        int dComp = 0, dPend = 0, dNS = 0;

        for (int j = 0; j < appStore.getCount(); j++) {
            string appDate = string(appStore.getAt(j).getDate());
            while (!appDate.empty() && (appDate.back() == ' ' || appDate.back() == '\r' || appDate.back() == '\n'))
                appDate.pop_back();

            if (appStore.getAt(j).getDoctorID() == did && appDate == todayStr) {
                string s = string(appStore.getAt(j).getStatus());
                if (s == "completed") dComp++;
                else if (s == "pending" || s == "scheduled") dPend++;
                else if (s == "no-show") dNS++;
            }
        }
        if (dComp + dPend + dNS > 0) {
            char dBuf[50];
            int k = 0;
            while (docStore.getAt(i).getName()[k] != '\0' && k < 49) { dBuf[k] = docStore.getAt(i).getName()[k]; k++; }
            dBuf[k] = '\0';
            manualTrim(dBuf);

            docRowStrings[totalDocRows] = string(dBuf) + "  Closed: " + to_string(dComp) + "  Pending: " + to_string(dPend) + "  No Show: " + to_string(dNS);
            totalDocRows++;
        }
    }
    int docStart = docPage * 5;
    for (int i = 0; i < 5; i++) {
        int idx = docStart + i;
        if (idx < totalDocRows) {
            doctorRows[i].setString(docRowStrings[idx]);
            doctorRows[i].setFillColor(sf::Color::Black);
        }
        else {
            doctorRows[i].setString(i == 0 && totalDocRows == 0 ? "  No activity today" : "");
        }
    }
}

void refreshPatAppointments(sf::Text* c1, sf::Text* c2, sf::Text* c3, sf::Text* c4, sf::Text* c5, sf::Text* c6, int patID, int page) {
    Storage<Appointment> aStore;
    FileHandler::loadAppointments(aStore);
    Storage<Doctor> dStore;
    FileHandler::loadDoctors(dStore);

    Appointment* pAppts = new Appointment[100];
    int count = 0;

    for (int i = 0; i < aStore.getCount(); i++) {
        if (aStore.getAt(i).getPatientID() == patID) {
            pAppts[count++] = aStore.getAt(i);
        }
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            string d1 = string(pAppts[j].getDate());
            string d2 = string(pAppts[j + 1].getDate());
            if (d1.length() >= 10 && d2.length() >= 10) {
                long v1 = stol(d1.substr(6, 4) + d1.substr(3, 2) + d1.substr(0, 2));
                long v2 = stol(d2.substr(6, 4) + d2.substr(3, 2) + d2.substr(0, 2));
                if (v1 > v2) swap(pAppts[j], pAppts[j + 1]);
            }
        }
    }

    int start = page * 10;
    for (int i = 0; i < 10; i++) {
        int idx = start + i;
        if (idx < count) {
            Appointment a = pAppts[idx];

            c1[i].setString(to_string(a.getID()));
            c4[i].setString(a.getDate());
            c5[i].setString(a.getTimeSlot());
            c6[i].setString(a.getStatus());

            char dName[50] = "Unknown";
            for (int d = 0; d < dStore.getCount(); d++) {
                if (dStore.getAt(d).getID() == a.getDoctorID()) {
                    int k = 0;
                    while (dStore.getAt(d).getName()[k] != '\0' && k < 49) { dName[k] = dStore.getAt(d).getName()[k]; k++; }
                    dName[k] = '\0'; manualTrim(dName);
                    break;
                }
            }
            c2[i].setString(dName);
            char spec[50] = "Unknown";
            for (int d = 0; d < dStore.getCount(); d++) {
                if (dStore.getAt(d).getID() == a.getDoctorID()) {
                    int k = 0;
                    while (dStore.getAt(d).getSpecialization()[k] != '\0' && k < 49) {
                        spec[k] = dStore.getAt(d).getSpecialization()[k]; k++;
                    }
                    spec[k] = '\0';
                    manualTrim(spec);
                    break;
                }
            }
            c3[i].setString(spec);

            c1[i].setFillColor(sf::Color::Black);
            c2[i].setFillColor(sf::Color::Black);
            c3[i].setFillColor(sf::Color::Black);
            c4[i].setFillColor(sf::Color::Black);
            c5[i].setFillColor(sf::Color::Black);
            c6[i].setFillColor(sf::Color::Black);
        }
        else {
            if (i == 0 && count == 0) {
                c1[i].setString("No appointments found for your ID.");
                c1[i].setFillColor(sf::Color::Red);
            }
            else {
                c1[i].setString("");
            }
            c2[i].setString(""); c3[i].setString(""); c4[i].setString(""); c5[i].setString(""); c6[i].setString("");
        }
    }
    delete[] pAppts;
}

void refreshPatRecords(sf::Text* c1, sf::Text* c2, sf::Text* c3, sf::Text* c4, int patID, int page) {
    Storage<Prescription> preStore; FileHandler::loadPrescription(preStore);
    Storage<Doctor> dStore; FileHandler::loadDoctors(dStore);

    Prescription* px = new Prescription[100]; int count = 0;
    for (int i = 0; i < preStore.getCount(); i++) if (preStore.getAt(i).getPatientID() == patID) px[count++] = preStore.getAt(i);

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            string d1 = string(px[j].getDate()); string d2 = string(px[j + 1].getDate());
            int v1 = stoi(d1.substr(6, 4)) * 10000 + stoi(d1.substr(3, 2)) * 100 + stoi(d1.substr(0, 2));
            int v2 = stoi(d2.substr(6, 4)) * 10000 + stoi(d2.substr(3, 2)) * 100 + stoi(d2.substr(0, 2));
            if (v1 < v2) swap(px[j], px[j + 1]);
        }
    }

    int start = page * 10;
    for (int i = 0; i < 10; i++) {
        if (start + i < count) {
            Prescription p = px[start + i];
            c1[i].setString(p.getDate());

            char dName[50] = "Unknown";
            for (int d = 0; d < dStore.getCount(); d++) if (dStore.getAt(d).getID() == p.getDoctorID()) {
                int k = 0; while (dStore.getAt(d).getName()[k] != '\0' && k < 49) { dName[k] = dStore.getAt(d).getName()[k]; k++; } dName[k] = '\0'; manualTrim(dName); break;
            }
            c2[i].setString(dName); c3[i].setString(p.getMedicines()); c4[i].setString(p.getNotes());
        }
        else { c1[i].setString(i == 0 && count == 0 ? "No records found." : ""); c2[i].setString(""); c3[i].setString(""); c4[i].setString(""); }
    }
    delete[] px;
}

void refreshPatBills(sf::Text* c1, sf::Text* c2, sf::Text* c3, sf::Text* c4, sf::Text* c5, int patID, int page, sf::Text& totalOut) {
    Storage<Bill> bStore; FileHandler::loadBills(bStore);
    Bill* pBills = new Bill[100]; int count = 0; float totalUnpaid = 0;

    for (int i = 0; i < bStore.getCount(); i++) {
        if (bStore.getAt(i).getPatientID() == patID) {
            pBills[count++] = bStore.getAt(i);
            string stat = string(bStore.getAt(i).getStatus()); while (!stat.empty() && isspace(stat.back())) stat.pop_back();
            if (stat == "unpaid") totalUnpaid += bStore.getAt(i).getAmount();
        }
    }
    totalOut.setString("Total Outstanding Unpaid: PKR " + to_string((int)totalUnpaid));

    int start = page * 10;
    for (int i = 0; i < 10; i++) {
        if (start + i < count) {
            c1[i].setString(to_string(pBills[start + i].getID())); c2[i].setString(to_string(pBills[start + i].getAppointmentID()));
            c3[i].setString(to_string((int)pBills[start + i].getAmount())); c4[i].setString(pBills[start + i].getStatus()); c5[i].setString(pBills[start + i].getDate());
        }
        else { c1[i].setString(i == 0 && count == 0 ? "No bills found." : ""); c2[i].setString(""); c3[i].setString(""); c4[i].setString(""); c5[i].setString(""); }
    }
    delete[] pBills;
}

bool manualCaseInsensitiveMatch(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        char a = str1[i]; if (a >= 'A' && a <= 'Z') a += 32;
        char b = str2[i]; if (b >= 'A' && b <= 'Z') b += 32;
        if (a != b) return false;
        i++;
    }
    return str1[i] == '\0' && str2[i] == '\0';
}