#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif
#include "GUI.h"
#include <cstdio>
#include <ctime>
#include <cmath>

// ─── TBox::draw ──────────────────────────────────────────────────────────────
void TBox::draw(sf::RenderWindow& w, float blink) {
    body.setOutlineColor(focus ? sf::Color(210, 70, 130) : sf::Color(225, 170, 200));
    body.setOutlineThickness(focus ? 2.f : 1.f);
    w.draw(body);
    if (len == 0 && !focus) { w.draw(ph); return; }
    char d[256];
    if (pass) { for (int i = 0; i < len; i++) d[i] = '*'; d[len] = '\0'; }
    else { Person::copyStr(d, buf, 256); }
    txt.setString(d);
    w.draw(txt);
    if (focus) {
        sf::FloatRect tb = txt.getGlobalBounds();
        sf::RectangleShape cur({ 2.f, 18.f });
        cur.setPosition(tb.left + tb.width + 2.f, body.getPosition().y + 12.f);
        cur.setFillColor(sf::Color(210, 70, 130, (sf::Uint8)blink));
        w.draw(cur);
    }
}

// ─── Constructor ─────────────────────────────────────────────────────────────
GUI::GUI()
    : win(sf::VideoMode(1280, 720),
        "MediCore - Hospital Management System",
        sf::Style::Close | sf::Style::Resize | sf::Style::Titlebar),
    scr(Scr::WELCOME),
    bT(0.f), bA(255.f),
    fbT(0.f), fbE(false),
    scroll(0.f),
    curPat(nullptr), curDoc(nullptr), fails(0),
    nBox(0), aBox(0),
    bkStep(0), bkDocId(0),
    prStep(0), prAptId(0),
    nFBtns(0)
{
    win.setFramerateLimit(60);
    // Lock the rendering view to logical 1280x720 — content will scale up
    // when the window is maximized/resized instead of leaving empty space.
    win.setView(sf::View(sf::FloatRect(0.f, 0.f, 1280.f, 720.f)));
    fbMsg[0] = '\0';
    bkSpec[0] = bkDate[0] = bkSlot[0] = '\0';
    prMed[0] = prNote[0] = '\0';

    const char* fp[] = {
        "C:/Windows/Fonts/segoeui.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/calibri.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/System/Library/Fonts/Helvetica.ttc",
        "arial.ttf"
    };
    for (auto& p : fp) if (font.loadFromFile(p)) break;

    initFiles();
    loadAll();
}

// ─── Data init ───────────────────────────────────────────────────────────────
void GUI::initFiles() {
    auto ex = [](const char* n) -> bool {
        FILE* f = fopen(n, "r");
        if (f) { fclose(f); return true; }
        return false;
        };
    if (!ex("admin.txt")) { FILE* f = fopen("admin.txt", "w");         if (f) { fputs("1,Admin,admin123\n", f); fclose(f); } }
    if (!ex("patients.txt")) { FILE* f = fopen("patients.txt", "w");      if (f) { fputs("1,Ahmed Ali,28,M,03001234567,pass123,5000.00\n", f); fclose(f); } }
    if (!ex("doctors.txt")) { FILE* f = fopen("doctors.txt", "w");       if (f) { fputs("1,Dr. Sara Khan,Cardiology,03111234567,doc456,1500.00\n", f); fclose(f); } }
    if (!ex("appointments.txt")) { FILE* f = fopen("appointments.txt", "w");  if (f) fclose(f); }
    if (!ex("bills.txt")) { FILE* f = fopen("bills.txt", "w");         if (f) fclose(f); }
    if (!ex("prescriptions.txt")) { FILE* f = fopen("prescriptions.txt", "w"); if (f) fclose(f); }
    if (!ex("security_log.txt")) { FILE* f = fopen("security_log.txt", "w");  if (f) fclose(f); }
    if (!ex("discharged.txt")) { FILE* f = fopen("discharged.txt", "w");    if (f) fclose(f); }
}

void GUI::loadAll() {
    PAT.clear(); DOC.clear(); APT.clear(); BIL.clear(); PRE.clear();
    try {
        FileHandler::loadPatients(PAT);
        FileHandler::loadDoctors(DOC);
        FileHandler::loadAdmin(adm);
        FileHandler::loadAppointments(APT);
        FileHandler::loadBills(BIL);
        FileHandler::loadPrescriptions(PRE);
    }
    catch (HospitalException& e) { setFB(e.what(), true); }
}

void GUI::setFB(const char* m, bool e) {
    Person::copyStr(fbMsg, m, 300);
    fbE = e;
    fbT = 4.f;
}

void GUI::goTo(Scr s, int nb) {
    scr = s;
    nBox = nb;
    aBox = 0;
    scroll = 0.f;
    fbMsg[0] = '\0';
}

// ─── ID generators ───────────────────────────────────────────────────────────
int GUI::nxtApt() {
    int m = 0;
    for (int i = 0; i < APT.size(); i++) if (APT.get(i).getAppointmentId() > m) m = APT.get(i).getAppointmentId();
    return m + 1;
}
int GUI::nxtBil() {
    int m = 0;
    for (int i = 0; i < BIL.size(); i++) if (BIL.get(i).getBillId() > m) m = BIL.get(i).getBillId();
    return m + 1;
}
int GUI::nxtPre() {
    int m = 0;
    for (int i = 0; i < PRE.size(); i++) if (PRE.get(i).getPrescriptionId() > m) m = PRE.get(i).getPrescriptionId();
    return m + 1;
}
int GUI::nxtDoc() {
    int m = 0;
    for (int i = 0; i < DOC.size(); i++) if (DOC.get(i).getId() > m) m = DOC.get(i).getId();
    return m + 1;
}

// ─── Sorting ─────────────────────────────────────────────────────────────────
void GUI::sortAptAsc(Appointment* a, int n) {
    for (int i = 0; i < n - 1; i++) for (int j = 0; j < n - i - 1; j++)
        if (Appointment::compareDates(a[j].getDate(), a[j + 1].getDate()) > 0) {
            Appointment t = a[j]; a[j] = a[j + 1]; a[j + 1] = t;
        }
}
void GUI::sortAptDesc(Appointment* a, int n) {
    for (int i = 0; i < n - 1; i++) for (int j = 0; j < n - i - 1; j++)
        if (Appointment::compareDates(a[j].getDate(), a[j + 1].getDate()) < 0) {
            Appointment t = a[j]; a[j] = a[j + 1]; a[j + 1] = t;
        }
}
void GUI::sortPreDesc(Prescription* a, int n) {
    for (int i = 0; i < n - 1; i++) for (int j = 0; j < n - i - 1; j++)
        if (Appointment::compareDates(a[j].getDate(), a[j + 1].getDate()) < 0) {
            Prescription t = a[j]; a[j] = a[j + 1]; a[j + 1] = t;
        }
}

bool GUI::slotFree(int dId, const char* date, const char* slot) {
    for (int i = 0; i < APT.size(); i++) {
        Appointment& a = APT.get(i);
        if (a.getDoctorId() == dId &&
            Person::compareStr(a.getDate(), date) == 0 &&
            Person::compareStr(a.getTimeSlot(), slot) == 0 &&
            Person::compareStr(a.getStatus(), "cancelled") != 0)
            return false;
    }
    return true;
}

Scr GUI::parentOf(Scr s) {
    switch (s) {
    case Scr::BOOK: case Scr::CANCEL_APT: case Scr::VIEW_APTS:
    case Scr::VIEW_REC: case Scr::VIEW_BILLS: case Scr::PAY_BILL:
    case Scr::TOPUP:
        return Scr::PAT_MENU;
    case Scr::TODAY: case Scr::MARK_DONE: case Scr::MARK_NS:
    case Scr::WRITE_PRESC: case Scr::PAT_HIST:
        return Scr::DOC_MENU;
    case Scr::ADD_DOC: case Scr::REM_DOC: case Scr::ALL_PAT:
    case Scr::ALL_DOC: case Scr::ALL_APT: case Scr::UNPAID:
    case Scr::DISCHARGE: case Scr::SEC_LOG: case Scr::REPORT:
        return Scr::ADM_MENU;
    default: return Scr::WELCOME;
    }
}

// ─── Drawing helpers ─────────────────────────────────────────────────────────
sf::Text GUI::mkT(const char* s, unsigned sz, sf::Color c, float x, float y) {
    sf::Text t;
    t.setFont(font);
    t.setString(s);
    t.setCharacterSize(sz);
    t.setFillColor(c);
    t.setPosition(x, y);
    return t;
}

void GUI::ctrT(sf::Text& t, float cx, float y) {
    sf::FloatRect b = t.getLocalBounds();
    t.setPosition(cx - b.width / 2.f - b.left, y);
}

void GUI::banner(const char* title, const char* sub) {
    sf::RectangleShape b1({ 1280.f, 98.f });
    b1.setFillColor(P::PRI()); b1.setPosition(0, 0); win.draw(b1);
    sf::RectangleShape b2({ 1280.f, 16.f });
    b2.setFillColor(P::PRID()); b2.setPosition(0, 84); win.draw(b2);
    for (int i = 0; i < 8; i++) {
        sf::CircleShape c(45.f + i * 16.f);
        c.setFillColor(sf::Color(255, 255, 255, 10));
        c.setPosition((float)(i * 170 - 45), -28.f);
        win.draw(c);
    }
    sf::Text tt = mkT(title, 26, P::WHITE(), 0, 20);
    tt.setStyle(sf::Text::Bold);
    ctrT(tt, 640, 20);
    win.draw(tt);
    if (sub && sub[0]) {
        sf::Text st = mkT(sub, 14, P::PRIL(), 0, 58);
        ctrT(st, 640, 58);
        win.draw(st);
    }
}

void GUI::drawCard(float x, float y, float w, float h) {
    sf::RectangleShape sh({ w, h });
    sh.setPosition(x + 3, y + 3);
    sh.setFillColor(P::SHADOW());
    win.draw(sh);
    sf::RectangleShape c({ w, h });
    c.setPosition(x, y);
    c.setFillColor(P::CARD());
    c.setOutlineColor(P::PRIL());
    c.setOutlineThickness(1.f);
    win.draw(c);
}

void GUI::drawFB() {
    if (fbT <= 0 || fbMsg[0] == '\0') return;
    sf::Color bg = fbE ? sf::Color(175, 30, 30, 235) : sf::Color(46, 120, 50, 235);
    sf::RectangleShape bar({ 1280.f, 38.f });
    bar.setPosition(0, 682);
    bar.setFillColor(bg);
    win.draw(bar);
    win.draw(mkT(fbMsg, 14, P::WHITE(), 18, 689));
}

void GUI::tblH(const char** c, const float* w, int n, float x, float y) {
    sf::RectangleShape h({ 1180.f, 32.f });
    h.setPosition(x, y);
    h.setFillColor(P::HDRTBL());
    win.draw(h);
    float cx = x + 8;
    for (int i = 0; i < n; i++) {
        sf::Text t = mkT(c[i], 13, P::WHITE(), cx, y + 8);
        t.setStyle(sf::Text::Bold);
        win.draw(t);
        cx += w[i];
    }
}

void GUI::tblR(const char** v, const float* w, int n, float x, float y, bool alt) {
    sf::RectangleShape row({ 1180.f, 28.f });
    row.setPosition(x, y);
    row.setFillColor(alt ? P::ROWA() : P::ROWB());
    win.draw(row);
    sf::RectangleShape brd({ 1180.f, 1.f });
    brd.setPosition(x, y + 27);
    brd.setFillColor(P::PRIL());
    win.draw(brd);
    float cx = x + 8;
    for (int i = 0; i < n; i++) {
        win.draw(mkT(v[i], 13, P::TXT(), cx, y + 7));
        cx += w[i];
    }
}

void GUI::mkBox(int i, float x, float y, float bw, float bh,
    const char* ph, bool pass, int ml) {
    TBox& b = boxes[i];
    b.clear();
    b.focus = false;
    b.pass = pass;
    b.maxL = ml;
    b.body.setPosition(x, y);
    b.body.setSize({ bw, bh });
    b.body.setFillColor(P::INPUTBG());
    b.body.setOutlineColor(P::BORDER());
    b.body.setOutlineThickness(1.f);
    b.ph = mkT(ph, 14, sf::Color(180, 130, 155), x + 10, y + (bh - 20) / 2.f);
    b.txt = mkT("", 14, P::TXT(), x + 10, y + (bh - 20) / 2.f);
}

void GUI::drawBoxes() {
    for (int i = 0; i < nBox; i++) boxes[i].draw(win, bA);
}

// ─── addBtn: render a button AND store it for hit detection ─────────────────
void GUI::addBtn(float x, float y, float w, float h, const char* lbl,
    BAct act, unsigned fs, sf::Color cn, sf::Color ch) {
    if (nFBtns >= MAX_BTNS) return;
    Btn& b = fBtns[nFBtns++];
    b.cn = cn; b.ch = ch; b.act = act;
    b.body.setPosition(x, y);
    b.body.setSize({ w, h });
    // CRITICAL: convert pixel mouse position to logical view coordinates
    // so hover detection works correctly when window is maximized/resized.
    sf::Vector2i pixelPos = sf::Mouse::getPosition(win);
    sf::Vector2f mp = win.mapPixelToCoords(pixelPos);
    b.hov = b.hit(mp);
    b.body.setFillColor(b.hov ? ch : cn);
    b.lbl = mkT(lbl, fs, P::WHITE(), 0, 0);
    sf::FloatRect tb = b.lbl.getLocalBounds();
    b.lbl.setPosition(x + (w - tb.width) / 2.f - tb.left,
        y + (h - tb.height) / 2.f - tb.top - 2.f);
    win.draw(b.body);
    win.draw(b.lbl);
}

void GUI::addBackBtn() {
    if (nFBtns >= MAX_BTNS) return;
    Btn& b = fBtns[nFBtns++];
    b.cn = sf::Color(255, 235, 245);
    b.ch = P::PRIL();
    b.act = BAct::BACK;
    b.body.setPosition(1105, 8);
    b.body.setSize({ 165, 34 });
    sf::Vector2i pixelPos = sf::Mouse::getPosition(win);
    sf::Vector2f mp = win.mapPixelToCoords(pixelPos);
    b.hov = b.hit(mp);
    b.body.setFillColor(b.hov ? b.ch : b.cn);
    b.body.setOutlineColor(P::PRI());
    b.body.setOutlineThickness(1.f);
    b.lbl = mkT("<  Back", 14, P::PRID(), 1130, 14);
    win.draw(b.body);
    win.draw(b.lbl);
}

// ─── Main loop ───────────────────────────────────────────────────────────────
void GUI::run() {
    sf::Clock clk;
    while (win.isOpen()) {
        float dt = clk.restart().asSeconds();
        handleEvents();
        update(dt);
        render();
    }
}

void GUI::update(float dt) {
    if (fbT > 0) fbT -= dt;
    bT += dt;
    if (bT > 1.f) bT = 0.f;
    bA = (bT < 0.5f) ? 255.f : 0.f;
}

// ─── Event handling ──────────────────────────────────────────────────────────
void GUI::handleEvents() {
    sf::Event e;
    while (win.pollEvent(e)) {
        if (e.type == sf::Event::Closed) { win.close(); return; }

        // window resize: keep logical 1280x720 view, scale to fill window
        if (e.type == sf::Event::Resized) {
            sf::View v(sf::FloatRect(0.f, 0.f, 1280.f, 720.f));
            win.setView(v);
        }

        // scroll
        if (e.type == sf::Event::MouseWheelScrolled) {
            scroll -= e.mouseWheelScroll.delta * 30.f;
            if (scroll < 0) scroll = 0;
        }

        // text input into focused box
        if (e.type == sf::Event::TextEntered && aBox < nBox && boxes[aBox].focus) {
            char c = (char)e.text.unicode;
            if (c == '\b') boxes[aBox].del();
            else if (c >= 32 && c < 127) boxes[aBox].add(c);
        }

        // Tab switches focus
        if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Tab && nBox > 0) {
            boxes[aBox].focus = false;
            aBox = (aBox + 1) % nBox;
            boxes[aBox].focus = true;
        }

        // Enter to submit
        if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter) {
            if (scr == Scr::LOGIN_P)      doLogin(1);
            else if (scr == Scr::LOGIN_D)      doLogin(2);
            else if (scr == Scr::LOGIN_A)      doLogin(3);
            else if (scr == Scr::BOOK)         doBook();
            else if (scr == Scr::CANCEL_APT)   doCancelApt();
            else if (scr == Scr::PAY_BILL)     doPayBill();
            else if (scr == Scr::TOPUP)        doTopup();
            else if (scr == Scr::MARK_DONE)    doMarkDone();
            else if (scr == Scr::MARK_NS)      doMarkNS();
            else if (scr == Scr::WRITE_PRESC)  doWritePresc();
            else if (scr == Scr::ADD_DOC)      doAddDoc();
            else if (scr == Scr::REM_DOC)      doRemDoc();
            else if (scr == Scr::DISCHARGE)    doDischarge();
        }

        // mouse click
        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mp = win.mapPixelToCoords({ e.mouseButton.x, e.mouseButton.y });

            // focus input boxes
            for (int i = 0; i < nBox; i++) {
                boxes[i].focus = boxes[i].hit(mp);
                if (boxes[i].focus) aBox = i;
            }

            // check buttons (last-frame's list, but it's fresh because render happens every frame)
            for (int i = 0; i < nFBtns; i++) {
                if (fBtns[i].hit(mp)) {
                    doAction(fBtns[i].act);
                    break;
                }
            }
        }
    }
}

// ─── Action dispatcher ───────────────────────────────────────────────────────
void GUI::doAction(BAct a) {
    switch (a) {
    case BAct::BACK: {
        Scr par = parentOf(scr);
        if (par != scr) { goTo(par); bkStep = 0; prStep = 0; }
        break;
    }

                   // Welcome
    case BAct::W_PATIENT:
        fails = 0; goTo(Scr::LOGIN_P, 2);
        mkBox(0, 390, 298, 500, 44, "Patient ID", false, 8);
        mkBox(1, 390, 380, 500, 44, "Password", true, 40);
        boxes[0].focus = true;
        break;
    case BAct::W_DOCTOR:
        fails = 0; goTo(Scr::LOGIN_D, 2);
        mkBox(0, 390, 298, 500, 44, "Doctor ID", false, 8);
        mkBox(1, 390, 380, 500, 44, "Password", true, 40);
        boxes[0].focus = true;
        break;
    case BAct::W_ADMIN:
        fails = 0; goTo(Scr::LOGIN_A, 2);
        mkBox(0, 390, 298, 500, 44, "Admin ID", false, 8);
        mkBox(1, 390, 380, 500, 44, "Password", true, 40);
        boxes[0].focus = true;
        break;
    case BAct::W_EXIT: win.close(); break;

        // Login
    case BAct::L_LOGIN:
        if (scr == Scr::LOGIN_P) doLogin(1);
        else if (scr == Scr::LOGIN_D) doLogin(2);
        else doLogin(3);
        break;
    case BAct::L_BACK: goTo(Scr::WELCOME); break;

        // Patient menu
    case BAct::PM_BOOK:
        bkStep = 0; bkSpec[0] = '\0'; bkDocId = 0; bkDate[0] = '\0'; bkSlot[0] = '\0';
        goTo(Scr::BOOK, 1);
        mkBox(0, 240, 340, 800, 44, "Enter specialization (e.g. Cardiology)");
        boxes[0].focus = true;
        break;
    case BAct::PM_CANCEL:
        goTo(Scr::CANCEL_APT, 1);
        mkBox(0, 390, 510, 460, 44, "Appointment ID to cancel");
        boxes[0].focus = true;
        break;
    case BAct::PM_VIEW_APTS:  goTo(Scr::VIEW_APTS); break;
    case BAct::PM_VIEW_REC:   goTo(Scr::VIEW_REC); break;
    case BAct::PM_VIEW_BILLS: goTo(Scr::VIEW_BILLS); break;
    case BAct::PM_PAY:
        goTo(Scr::PAY_BILL, 1);
        mkBox(0, 390, 510, 460, 44, "Bill ID to pay");
        boxes[0].focus = true;
        break;
    case BAct::PM_TOPUP:
        goTo(Scr::TOPUP, 1);
        mkBox(0, 440, 320, 360, 44, "Amount (PKR)");
        boxes[0].focus = true;
        break;
    case BAct::PM_LOGOUT: curPat = nullptr; fails = 0; goTo(Scr::WELCOME); break;

        // Doctor menu
    case BAct::DM_TODAY: goTo(Scr::TODAY); break;
    case BAct::DM_DONE:
        goTo(Scr::MARK_DONE, 1);
        mkBox(0, 440, 460, 360, 44, "Appointment ID");
        boxes[0].focus = true;
        break;
    case BAct::DM_NS:
        goTo(Scr::MARK_NS, 1);
        mkBox(0, 440, 460, 360, 44, "Appointment ID");
        boxes[0].focus = true;
        break;
    case BAct::DM_PRESC:
        prStep = 0; prAptId = 0; prMed[0] = '\0'; prNote[0] = '\0';
        goTo(Scr::WRITE_PRESC, 1);
        mkBox(0, 240, 270, 800, 44, "Appointment ID (must be completed)");
        boxes[0].focus = true;
        break;
    case BAct::DM_HIST:
        goTo(Scr::PAT_HIST, 1);
        mkBox(0, 440, 160, 360, 44, "Patient ID");
        boxes[0].focus = true;
        break;
    case BAct::DM_LOGOUT: curDoc = nullptr; fails = 0; goTo(Scr::WELCOME); break;

        // Admin menu
    case BAct::AM_ADD:
        goTo(Scr::ADD_DOC, 5);
        mkBox(0, 350, 178, 560, 38, "Name");
        mkBox(1, 350, 248, 560, 38, "Specialization");
        mkBox(2, 350, 318, 560, 38, "Contact (11 digits)");
        mkBox(3, 350, 388, 560, 38, "Password (min 6 chars)", true);
        mkBox(4, 350, 458, 560, 38, "Fee (PKR)");
        boxes[0].focus = true;
        break;
    case BAct::AM_REM:
        goTo(Scr::REM_DOC, 1);
        mkBox(0, 440, 510, 360, 44, "Doctor ID to remove");
        boxes[0].focus = true;
        break;
    case BAct::AM_PAT:    goTo(Scr::ALL_PAT); break;
    case BAct::AM_DOC:    goTo(Scr::ALL_DOC); break;
    case BAct::AM_APT:    goTo(Scr::ALL_APT); break;
    case BAct::AM_UNPAID: goTo(Scr::UNPAID); break;
    case BAct::AM_DISC:
        goTo(Scr::DISCHARGE, 1);
        mkBox(0, 440, 510, 360, 44, "Patient ID to discharge");
        boxes[0].focus = true;
        break;
    case BAct::AM_LOG:    goTo(Scr::SEC_LOG); break;
    case BAct::AM_REPORT: goTo(Scr::REPORT); break;
    case BAct::AM_LOGOUT: goTo(Scr::WELCOME); break;

        // Submit (context-dependent)
    case BAct::SUBMIT:
        if (scr == Scr::BOOK)         doBook();
        else if (scr == Scr::CANCEL_APT)   doCancelApt();
        else if (scr == Scr::PAY_BILL)     doPayBill();
        else if (scr == Scr::TOPUP)        doTopup();
        else if (scr == Scr::MARK_DONE)    doMarkDone();
        else if (scr == Scr::MARK_NS)      doMarkNS();
        else if (scr == Scr::WRITE_PRESC)  doWritePresc();
        else if (scr == Scr::ADD_DOC)      doAddDoc();
        else if (scr == Scr::REM_DOC)      doRemDoc();
        else if (scr == Scr::DISCHARGE)    doDischarge();
        break;

        // Slots
    case BAct::SLOT_0: case BAct::SLOT_1: case BAct::SLOT_2: case BAct::SLOT_3:
    case BAct::SLOT_4: case BAct::SLOT_5: case BAct::SLOT_6: case BAct::SLOT_7: {
        int si = (int)a - (int)BAct::SLOT_0;
        const char* slots[] = { "09:00","10:00","11:00","12:00","13:00","14:00","15:00","16:00" };
        if (slotFree(bkDocId, bkDate, slots[si])) {
            Person::copyStr(bkSlot, slots[si], 10);
            boxes[0].clear();
            for (int k = 0; k < Person::strLen(slots[si]); k++) boxes[0].add(slots[si][k]);
        }
        else {
            setFB("That slot is already taken.", true);
        }
        break;
    }

    case BAct::NONE: default: break;
    }
}

// ─── Render dispatcher ───────────────────────────────────────────────────────
void GUI::render() {
    nFBtns = 0;
    win.clear(P::BG());

    switch (scr) {
    case Scr::WELCOME:     rWelcome(); break;
    case Scr::LOGIN_P:     rLogin("Patient Login"); break;
    case Scr::LOGIN_D:     rLogin("Doctor Login"); break;
    case Scr::LOGIN_A:     rLogin("Admin Login"); break;
    case Scr::PAT_MENU:    rPatMenu(); break;
    case Scr::DOC_MENU:    rDocMenu(); break;
    case Scr::ADM_MENU:    rAdmMenu(); break;
    case Scr::BOOK:        rBook(); break;
    case Scr::CANCEL_APT:  rCancelApt(); break;
    case Scr::VIEW_APTS:   rViewApts(); break;
    case Scr::VIEW_REC:    rViewRec(); break;
    case Scr::VIEW_BILLS:  rViewBills(); break;
    case Scr::PAY_BILL:    rPayBill(); break;
    case Scr::TOPUP:       rTopup(); break;
    case Scr::TODAY:       rToday(); break;
    case Scr::MARK_DONE:   rMarkDone(); break;
    case Scr::MARK_NS:     rMarkNS(); break;
    case Scr::WRITE_PRESC: rWritePresc(); break;
    case Scr::PAT_HIST:    rPatHist(); break;
    case Scr::ADD_DOC:     rAddDoc(); break;
    case Scr::REM_DOC:     rRemDoc(); break;
    case Scr::ALL_PAT:     rAllPat(); break;
    case Scr::ALL_DOC:     rAllDoc(); break;
    case Scr::ALL_APT:     rAllApt(); break;
    case Scr::UNPAID:      rUnpaid(); break;
    case Scr::DISCHARGE:   rDischarge(); break;
    case Scr::SEC_LOG:     rSecLog(); break;
    case Scr::REPORT:      rReport(); break;
    }

    drawFB();
    win.display();
}

// ═══════════════════════════════════════════════════════════════════════════
//  BUSINESS LOGIC
// ═══════════════════════════════════════════════════════════════════════════

void GUI::doLogin(int role) {
    int id = Person::strToInt(boxes[0].get());
    const char* pw = boxes[1].get();
    char idStr[12]; Person::intToStr(id, idStr, 12);

    bool success = false;
    const char* roleStr = nullptr;

    if (role == 1) {
        Patient* p = PAT.findById(id);
        if (p && Person::compareStr(p->getPassword(), pw) == 0) {
            curPat = p; fails = 0; goTo(Scr::PAT_MENU);
            char msg[80] = "Welcome, ";
            Person::copyStr(msg + 9, p->getName(), 71);
            setFB(msg, false);
            success = true;
        }
        roleStr = "Patient";
    }
    else if (role == 2) {
        Doctor* d = DOC.findById(id);
        if (d && Person::compareStr(d->getPassword(), pw) == 0) {
            curDoc = d; fails = 0; goTo(Scr::DOC_MENU);
            char msg[80] = "Welcome, Dr. ";
            Person::copyStr(msg + 13, d->getName(), 67);
            setFB(msg, false);
            success = true;
        }
        roleStr = "Doctor";
    }
    else {
        if (adm.getId() == id && Person::compareStr(adm.getPassword(), pw) == 0) {
            fails = 0; goTo(Scr::ADM_MENU);
            setFB("Welcome, Admin!", false);
            success = true;
        }
        roleStr = "Admin";
    }

    if (!success) {
        ++fails;
        FileHandler::logSecurityEvent(roleStr, idStr, "FAILED");
        if (fails >= 3) {
            setFB("Account locked. Contact admin.", true);
            goTo(Scr::WELCOME);
        }
        else {
            char m[60] = "Wrong credentials. Attempt ";
            char ab[4]; Person::intToStr(fails, ab, 4);
            Person::copyStr(m + Person::strLen(m), ab, 33);
            Person::copyStr(m + Person::strLen(m), " / 3", 5);
            setFB(m, true);
        }
    }
}

void GUI::doBook() {
    if (bkStep == 0) {
        Person::copyStr(bkSpec, boxes[0].get(), 60);
        Person::trim(bkSpec);
        if (bkSpec[0] == '\0') { setFB("Enter a specialization.", true); return; }
        bool found = false;
        for (int i = 0; i < DOC.size(); i++)
            if (Person::compareStrCI(DOC.get(i).getSpecialization(), bkSpec) == 0) { found = true; break; }
        if (!found) { setFB("No doctors found for that specialization.", true); return; }
        bkStep = 1;
        boxes[0].clear();
        mkBox(0, 390, 460, 460, 44, "Enter Doctor ID");
        boxes[0].focus = true;
        setFB("Doctors listed. Enter Doctor ID.", false);
    }
    else if (bkStep == 1) {
        bkDocId = Person::strToInt(boxes[0].get());
        Doctor* d = DOC.findById(bkDocId);
        if (!d || Person::compareStrCI(d->getSpecialization(), bkSpec) != 0) {
            setFB("Doctor not found in that specialization.", true); return;
        }
        bkStep = 2;
        boxes[0].clear();
        mkBox(0, 390, 320, 460, 44, "Date (DD-MM-YYYY)");
        boxes[0].focus = true;
        setFB("Doctor selected. Enter date.", false);
    }
    else if (bkStep == 2) {
        Person::copyStr(bkDate, boxes[0].get(), 20);
        Person::trim(bkDate);
        if (!Validator::validateDate(bkDate, FileHandler::getCurrentYear())) {
            setFB("Invalid date. Use DD-MM-YYYY, current or future year.", true); return;
        }
        bkStep = 3;
        boxes[0].clear();
        mkBox(0, 390, 580, 460, 44, "Type slot or click above (e.g. 09:00)");
        boxes[0].focus = true;
        setFB("Click an available slot or type it.", false);
    }
    else if (bkStep == 3) {
        Person::copyStr(bkSlot, boxes[0].get(), 10);
        Person::trim(bkSlot);
        if (!Validator::validateTimeSlot(bkSlot)) { setFB("Invalid slot. Use 09:00-16:00.", true); return; }
        if (!slotFree(bkDocId, bkDate, bkSlot)) { setFB("That slot is taken. Choose another.", true); return; }
        Doctor* d = DOC.findById(bkDocId);
        if (!d) { setFB("Doctor error.", true); return; }
        if (curPat->getBalance() < d->getFee()) {
            try { throw InsufficientFundsException(d->getFee(), curPat->getBalance()); }
            catch (InsufficientFundsException& ex) { setFB(ex.what(), true); }
            return;
        }
        *curPat -= d->getFee();
        char today[11]; FileHandler::getTodayDate(today);
        int aId = nxtApt();
        Appointment na(aId, curPat->getId(), bkDocId, bkDate, bkSlot, "pending");
        APT.add(na); FileHandler::appendAppointment(na);
        int bId = nxtBil();
        Bill nb(bId, curPat->getId(), aId, d->getFee(), "unpaid", today);
        BIL.add(nb); FileHandler::appendBill(nb);
        Patient* sp = PAT.findById(curPat->getId());
        if (sp) sp->setBalance(curPat->getBalance());
        FileHandler::updatePatient(PAT);
        char msg[80] = "Appointment booked! ID: ";
        char ab[10]; Person::intToStr(aId, ab, 10);
        Person::copyStr(msg + Person::strLen(msg), ab, 56);
        setFB(msg, false);
        bkStep = 0; bkSpec[0] = '\0'; bkDocId = 0; bkDate[0] = '\0'; bkSlot[0] = '\0';
        boxes[0].clear();
        mkBox(0, 240, 340, 800, 44, "Enter specialization (e.g. Cardiology)");
        boxes[0].focus = true;
    }
}

void GUI::doCancelApt() {
    int id = Person::strToInt(boxes[0].get());
    for (int i = 0; i < APT.size(); i++) {
        Appointment& a = APT.get(i);
        if (a.getAppointmentId() == id && a.getPatientId() == curPat->getId() &&
            Person::compareStr(a.getStatus(), "pending") == 0) {
            Doctor* d = DOC.findById(a.getDoctorId());
            float ref = d ? d->getFee() : 0;
            a.setStatus("cancelled");
            FileHandler::updateAppointment(APT);
            *curPat += ref;
            Patient* sp = PAT.findById(curPat->getId());
            if (sp) sp->setBalance(curPat->getBalance());
            FileHandler::updatePatient(PAT);
            for (int j = 0; j < BIL.size(); j++)
                if (BIL.get(j).getAppointmentId() == id &&
                    Person::compareStr(BIL.get(j).getStatus(), "unpaid") == 0) {
                    BIL.get(j).setStatus("cancelled"); break;
                }
            FileHandler::updateBill(BIL);
            char msg[80] = "Cancelled. PKR ";
            char rb[20]; Person::floatToStr(ref, rb, 20, 2);
            Person::copyStr(msg + Person::strLen(msg), rb, 65);
            Person::copyStr(msg + Person::strLen(msg), " refunded.", 10);
            setFB(msg, false);
            boxes[0].clear();
            return;
        }
    }
    setFB("Invalid appointment ID.", true);
}

void GUI::doPayBill() {
    int id = Person::strToInt(boxes[0].get());
    for (int i = 0; i < BIL.size(); i++) {
        Bill& b = BIL.get(i);
        if (b.getBillId() == id && b.getPatientId() == curPat->getId() &&
            Person::compareStr(b.getStatus(), "unpaid") == 0) {
            if (curPat->getBalance() < b.getAmount()) {
                try { throw InsufficientFundsException(b.getAmount(), curPat->getBalance()); }
                catch (InsufficientFundsException& ex) { setFB(ex.what(), true); }
                return;
            }
            *curPat -= b.getAmount();
            b.setStatus("paid");
            FileHandler::updateBill(BIL);
            Patient* sp = PAT.findById(curPat->getId());
            if (sp) sp->setBalance(curPat->getBalance());
            FileHandler::updatePatient(PAT);
            char msg[80] = "Bill paid. Balance: PKR ";
            char bb[20]; Person::floatToStr(curPat->getBalance(), bb, 20, 2);
            Person::copyStr(msg + Person::strLen(msg), bb, 56);
            setFB(msg, false);
            boxes[0].clear();
            return;
        }
    }
    setFB("Invalid bill ID.", true);
}

void GUI::doTopup() {
    const char* s = boxes[0].get();
    if (!Validator::validatePositiveFloat(s)) {
        try { throw InvalidInputException("Invalid amount. Must be a positive number."); }
        catch (InvalidInputException& ex) { setFB(ex.what(), true); }
        return;
    }
    float amt = Person::strToFloat(s);
    *curPat += amt;
    Patient* sp = PAT.findById(curPat->getId());
    if (sp) sp->setBalance(curPat->getBalance());
    FileHandler::updatePatient(PAT);
    char msg[80] = "Balance updated: PKR ";
    char bb[20]; Person::floatToStr(curPat->getBalance(), bb, 20, 2);
    Person::copyStr(msg + Person::strLen(msg), bb, 59);
    setFB(msg, false);
    boxes[0].clear();
}

void GUI::doMarkDone() {
    char today[11]; FileHandler::getTodayDate(today);
    int id = Person::strToInt(boxes[0].get());
    for (int i = 0; i < APT.size(); i++) {
        Appointment& a = APT.get(i);
        if (a.getAppointmentId() == id && a.getDoctorId() == curDoc->getId() &&
            Person::compareStr(a.getStatus(), "pending") == 0 &&
            Person::compareStr(a.getDate(), today) == 0) {
            a.setStatus("completed");
            FileHandler::updateAppointment(APT);
            setFB("Appointment marked as completed.", false);
            boxes[0].clear();
            return;
        }
    }
    setFB("Invalid ID or not today/pending.", true);
}

void GUI::doMarkNS() {
    char today[11]; FileHandler::getTodayDate(today);
    int id = Person::strToInt(boxes[0].get());
    for (int i = 0; i < APT.size(); i++) {
        Appointment& a = APT.get(i);
        if (a.getAppointmentId() == id && a.getDoctorId() == curDoc->getId() &&
            Person::compareStr(a.getStatus(), "pending") == 0 &&
            Person::compareStr(a.getDate(), today) == 0) {
            a.setStatus("noshow");
            FileHandler::updateAppointment(APT);
            for (int j = 0; j < BIL.size(); j++)
                if (BIL.get(j).getAppointmentId() == id &&
                    Person::compareStr(BIL.get(j).getStatus(), "unpaid") == 0) {
                    BIL.get(j).setStatus("cancelled"); break;
                }
            FileHandler::updateBill(BIL);
            setFB("Appointment marked as no-show.", false);
            boxes[0].clear();
            return;
        }
    }
    setFB("Invalid ID or not today/pending.", true);
}

void GUI::doWritePresc() {
    if (prStep == 0) {
        prAptId = Person::strToInt(boxes[0].get());
        bool ok = false;
        for (int i = 0; i < APT.size(); i++) {
            Appointment& a = APT.get(i);
            if (a.getAppointmentId() == prAptId && a.getDoctorId() == curDoc->getId() &&
                Person::compareStr(a.getStatus(), "completed") == 0) {
                ok = true; break;
            }
        }
        if (!ok) { setFB("Appointment not found or not completed.", true); return; }
        for (int i = 0; i < PRE.size(); i++)
            if (PRE.get(i).getAppointmentId() == prAptId) {
                setFB("Prescription already written.", true); return;
            }
        prStep = 1; nBox = 2;
        mkBox(1, 240, 370, 800, 44, "Medicines (e.g. Paracetamol 500mg;Amoxicillin 250mg)", false, 499);
        boxes[1].focus = true; aBox = 1;
        setFB("Valid appointment. Now enter medicines.", false);
    }
    else if (prStep == 1) {
        Person::copyStr(prMed, boxes[1].get(), 500);
        if (prMed[0] == '\0') { setFB("Medicines cannot be empty.", true); return; }
        prStep = 2; nBox = 3;
        mkBox(2, 240, 470, 800, 44, "Notes (max 300 chars)", false, 299);
        boxes[2].focus = true; aBox = 2;
        setFB("Now enter notes.", false);
    }
    else if (prStep == 2) {
        Person::copyStr(prNote, boxes[2].get(), 300);
        Appointment* apt = nullptr;
        for (int i = 0; i < APT.size(); i++)
            if (APT.get(i).getAppointmentId() == prAptId) { apt = &APT.get(i); break; }
        if (!apt) { setFB("Appointment error.", true); return; }
        int pid = nxtPre();
        Prescription pr(pid, prAptId, apt->getPatientId(), curDoc->getId(),
            apt->getDate(), prMed, prNote);
        PRE.add(pr); FileHandler::appendPrescription(pr);
        setFB("Prescription saved!", false);
        prStep = 0; prAptId = 0; prMed[0] = '\0'; prNote[0] = '\0';
        nBox = 1; boxes[0].clear();
        mkBox(0, 240, 270, 800, 44, "Appointment ID (must be completed)");
        boxes[0].focus = true; aBox = 0;
    }
}

void GUI::doAddDoc() {
    const char* nm = boxes[0].get();
    const char* sp = boxes[1].get();
    const char* ct = boxes[2].get();
    const char* pw = boxes[3].get();
    const char* fe = boxes[4].get();
    if (!Validator::validateContact(ct)) { setFB("Contact must be exactly 11 digits.", true); return; }
    if (!Validator::validatePassword(pw)) { setFB("Password min 6 characters.", true); return; }
    if (!Validator::validatePositiveFloat(fe)) { setFB("Fee must be a positive number.", true); return; }
    int newId = nxtDoc();
    Doctor d(newId, nm, sp, ct, pw, Person::strToFloat(fe));
    DOC.add(d); FileHandler::appendDoctor(d);
    char msg[80] = "Doctor added. ID: ";
    char ib[10]; Person::intToStr(newId, ib, 10);
    Person::copyStr(msg + Person::strLen(msg), ib, 62);
    setFB(msg, false);
    for (int i = 0; i < 5; i++) boxes[i].clear();
}

void GUI::doRemDoc() {
    int id = Person::strToInt(boxes[0].get());
    if (!DOC.findById(id)) { setFB("Doctor not found.", true); return; }
    for (int i = 0; i < APT.size(); i++)
        if (APT.get(i).getDoctorId() == id &&
            Person::compareStr(APT.get(i).getStatus(), "pending") == 0) {
            setFB("Cannot remove: doctor has pending appointments.", true);
            return;
        }
    FileHandler::deleteDoctorById(id, DOC);
    setFB("Doctor removed successfully.", false);
    boxes[0].clear();
}

void GUI::doDischarge() {
    int pid = Person::strToInt(boxes[0].get());
    Patient* p = PAT.findById(pid);
    if (!p) { setFB("Patient not found.", true); return; }
    for (int i = 0; i < BIL.size(); i++)
        if (BIL.get(i).getPatientId() == pid &&
            Person::compareStr(BIL.get(i).getStatus(), "unpaid") == 0) {
            setFB("Cannot discharge: unpaid bills exist.", true); return;
        }
    for (int i = 0; i < APT.size(); i++)
        if (APT.get(i).getPatientId() == pid &&
            Person::compareStr(APT.get(i).getStatus(), "pending") == 0) {
            setFB("Cannot discharge: pending appointments exist.", true); return;
        }
    FileHandler::archivePatient(*p, APT, BIL, PRE, pid);
    for (int i = APT.size() - 1; i >= 0; i--) if (APT.get(i).getPatientId() == pid) APT.removeAt(i);
    FileHandler::updateAppointment(APT);
    for (int i = BIL.size() - 1; i >= 0; i--) if (BIL.get(i).getPatientId() == pid) BIL.removeAt(i);
    FileHandler::updateBill(BIL);
    for (int i = PRE.size() - 1; i >= 0; i--) if (PRE.get(i).getPatientId() == pid) PRE.removeAt(i);
    FileHandler::updatePrescription(PRE);
    FileHandler::deletePatientById(pid, PAT);
    setFB("Patient discharged and archived.", false);
    boxes[0].clear();
}
