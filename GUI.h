#ifndef GUI_H
#define GUI_H

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

#include <SFML/Graphics.hpp>
#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "FileHandler.h"
#include "Validator.h"
#include "HospitalException.h"

// ── Colour palette ────────────────────────────────────────────────────────────
namespace P {
    inline sf::Color BG() { return sf::Color(250, 240, 245); }
    inline sf::Color CARD() { return sf::Color(255, 255, 255); }
    inline sf::Color PRI() { return sf::Color(210, 70, 130); }
    inline sf::Color PRID() { return sf::Color(165, 35, 95); }
    inline sf::Color PRIL() { return sf::Color(255, 200, 225); }
    inline sf::Color PRIEL() { return sf::Color(255, 240, 248); }
    inline sf::Color TXT() { return sf::Color(50, 20, 35); }
    inline sf::Color TXTM() { return sf::Color(130, 70, 100); }
    inline sf::Color WHITE() { return sf::Color(255, 255, 255); }
    inline sf::Color GREEN() { return sf::Color(67, 160, 71); }
    inline sf::Color GREEND() { return sf::Color(40, 120, 45); }
    inline sf::Color RED() { return sf::Color(198, 40, 40); }
    inline sf::Color REDD() { return sf::Color(150, 20, 20); }
    inline sf::Color SHADOW() { return sf::Color(210, 70, 130, 28); }
    inline sf::Color ROWA() { return sf::Color(255, 243, 250); }
    inline sf::Color ROWB() { return sf::Color(255, 255, 255); }
    inline sf::Color HDRTBL() { return sf::Color(210, 70, 130); }
    inline sf::Color BORDER() { return sf::Color(225, 170, 200); }
    inline sf::Color INPUTBG() { return sf::Color(255, 252, 254); }
}

// ── Screen enum ───────────────────────────────────────────────────────────────
enum class Scr {
    WELCOME, LOGIN_P, LOGIN_D, LOGIN_A,
    PAT_MENU, DOC_MENU, ADM_MENU,
    BOOK, CANCEL_APT, VIEW_APTS, VIEW_REC, VIEW_BILLS, PAY_BILL, TOPUP,
    TODAY, MARK_DONE, MARK_NS, WRITE_PRESC, PAT_HIST,
    ADD_DOC, REM_DOC, ALL_PAT, ALL_DOC, ALL_APT,
    UNPAID, DISCHARGE, SEC_LOG, REPORT
};

// ── Button action enum (used to identify which button was clicked) ────────────
enum class BAct {
    NONE, BACK,
    W_PATIENT, W_DOCTOR, W_ADMIN, W_EXIT,
    L_LOGIN, L_BACK,
    PM_BOOK, PM_CANCEL, PM_VIEW_APTS, PM_VIEW_REC, PM_VIEW_BILLS, PM_PAY, PM_TOPUP, PM_LOGOUT,
    DM_TODAY, DM_DONE, DM_NS, DM_PRESC, DM_HIST, DM_LOGOUT,
    AM_ADD, AM_REM, AM_PAT, AM_DOC, AM_APT, AM_UNPAID, AM_DISC, AM_LOG, AM_REPORT, AM_LOGOUT,
    SUBMIT,
    SLOT_0, SLOT_1, SLOT_2, SLOT_3, SLOT_4, SLOT_5, SLOT_6, SLOT_7
};

// ── Input box widget ──────────────────────────────────────────────────────────
struct TBox {
    sf::RectangleShape body;
    sf::Text           txt, ph;
    char               buf[256];
    int                len;
    int                maxL;
    bool               focus;
    bool               pass;

    TBox() : len(0), maxL(60), focus(false), pass(false) { buf[0] = '\0'; }
    void clear() { len = 0; buf[0] = '\0'; }
    const char* get() const { return buf; }
    void add(char c) { if (len < maxL - 1 && len < 255) { buf[len++] = c; buf[len] = '\0'; } }
    void del() { if (len > 0) buf[--len] = '\0'; }
    bool hit(sf::Vector2f m) const { return body.getGlobalBounds().contains(m); }
    void draw(sf::RenderWindow& w, float blink);
};

// ── Button widget — stores its own bounds + action ───────────────────────────
struct Btn {
    sf::RectangleShape body;
    sf::Text           lbl;
    sf::Color          cn, ch;
    bool               hov;
    BAct               act;

    Btn() : cn(sf::Color::Black), ch(sf::Color::Black), hov(false), act(BAct::NONE) {}
    bool hit(sf::Vector2f m) const { return body.getGlobalBounds().contains(m); }
};

// ── GUI class ─────────────────────────────────────────────────────────────────
class GUI {
public:
    GUI();
    ~GUI() {}
    void run();

private:
    sf::RenderWindow win;
    sf::Font         font;
    Scr              scr;

    // animation
    float bT, bA;
    float fbT;
    bool  fbE;
    char  fbMsg[300];
    float scroll;

    // data stores
    Storage<Patient>      PAT;
    Storage<Doctor>       DOC;
    Storage<Appointment>  APT;
    Storage<Bill>         BIL;
    Storage<Prescription> PRE;
    Admin                 adm;

    // session
    Patient* curPat;
    Doctor* curDoc;
    int      fails;

    // input boxes pool (max 6 per screen)
    TBox boxes[6];
    int  nBox, aBox;

    // booking flow state
    int  bkStep, bkDocId;
    char bkSpec[60], bkDate[20], bkSlot[10];

    // prescription flow state
    int  prStep, prAptId;
    char prMed[500], prNote[300];

    // per-frame button list (rebuilt every render)
    static const int MAX_BTNS = 24;
    Btn  fBtns[MAX_BTNS];
    int  nFBtns;

    // ── helpers ──────────────────────────────────────────────────────────
    void initFiles();
    void loadAll();
    void setFB(const char* m, bool e = false);
    void goTo(Scr s, int nb = 0);

    // ── main loop ────────────────────────────────────────────────────────
    void handleEvents();
    void update(float dt);
    void render();
    void doAction(BAct a);

    // ── widget builders ──────────────────────────────────────────────────
    void   addBtn(float x, float y, float w, float h, const char* lbl,
        BAct act, unsigned fs = 17,
        sf::Color cn = sf::Color(210, 70, 130),
        sf::Color ch = sf::Color(165, 35, 95));
    void   addBackBtn();
    sf::Text mkT(const char* s, unsigned sz, sf::Color c, float x, float y);
    void   ctrT(sf::Text& t, float cx, float y);
    void   banner(const char* title, const char* sub = "");
    void   drawCard(float x, float y, float w, float h);
    void   drawFB();
    void   tblH(const char** c, const float* w, int n, float x, float y);
    void   tblR(const char** v, const float* w, int n, float x, float y, bool alt);
    void   mkBox(int i, float x, float y, float bw, float bh,
        const char* ph, bool pass = false, int ml = 55);
    void   drawBoxes();

    // ── render screens ────────────────────────────────────────────────────
    void rWelcome();
    void rLogin(const char* role);
    void rPatMenu();
    void rDocMenu();
    void rAdmMenu();
    void rBook();
    void rCancelApt();
    void rViewApts();
    void rViewRec();
    void rViewBills();
    void rPayBill();
    void rTopup();
    void rToday();
    void rMarkDone();
    void rMarkNS();
    void rWritePresc();
    void rPatHist();
    void rAddDoc();
    void rRemDoc();
    void rAllPat();
    void rAllDoc();
    void rAllApt();
    void rUnpaid();
    void rDischarge();
    void rSecLog();
    void rReport();

    // ── business logic ────────────────────────────────────────────────────
    void doLogin(int role);
    void doBook();
    void doCancelApt();
    void doPayBill();
    void doTopup();
    void doMarkDone();
    void doMarkNS();
    void doWritePresc();
    void doAddDoc();
    void doRemDoc();
    void doDischarge();

    // ── misc helpers ──────────────────────────────────────────────────────
    Scr  parentOf(Scr s);
    bool slotFree(int dId, const char* date, const char* slot);
    int  nxtApt();
    int  nxtBil();
    int  nxtPre();
    int  nxtDoc();
    static void sortAptAsc(Appointment* a, int n);
    static void sortAptDesc(Appointment* a, int n);
    static void sortPreDesc(Prescription* a, int n);
};

#endif
