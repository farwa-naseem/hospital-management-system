#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif
#include "GUI.h"
#include <cstdio>
#include <ctime>

// ─── WELCOME ─────────────────────────────────────────────────────────────────
void GUI::rWelcome() {
    sf::RectangleShape hero({ 1280.f, 220.f });
    hero.setFillColor(P::PRI()); win.draw(hero);
    sf::RectangleShape acc({ 1280.f, 20.f });
    acc.setFillColor(P::PRID()); acc.setPosition(0, 200); win.draw(acc);
    for (int i = 0; i < 9; i++) {
        sf::CircleShape c(55.f + i * 18.f);
        c.setFillColor(sf::Color(255, 255, 255, 10));
        c.setPosition((float)(i * 155 - 55), -35.f);
        win.draw(c);
    }
    sf::Text ht = mkT("MediCore", 52, P::WHITE(), 0, 34);
    ht.setStyle(sf::Text::Bold); ctrT(ht, 640, 34); win.draw(ht);
    sf::Text st = mkT("Hospital Management System", 20, P::PRIL(), 0, 106);
    ctrT(st, 640, 106); win.draw(st);
    sf::Text su = mkT("Select your role to continue", 15, sf::Color(255, 210, 230), 0, 148);
    ctrT(su, 640, 148); win.draw(su);

    drawCard(340, 245, 600, 350);
    sf::Text lt = mkT("Login As", 20, P::PRID(), 0, 262);
    lt.setStyle(sf::Text::Bold); ctrT(lt, 640, 262); win.draw(lt);
    sf::RectangleShape div({ 520.f, 1.f });
    div.setPosition(380, 298); div.setFillColor(P::PRIL()); win.draw(div);

    addBtn(390, 310, 500, 52, "  Patient Login", BAct::W_PATIENT, 18, P::PRI(), P::PRID());
    addBtn(390, 374, 500, 52, "  Doctor Login", BAct::W_DOCTOR, 18, sf::Color(185, 50, 115), sf::Color(145, 20, 85));
    addBtn(390, 438, 500, 52, "  Admin Login", BAct::W_ADMIN, 18, sf::Color(155, 35, 90), sf::Color(120, 15, 65));
    addBtn(390, 502, 500, 52, "  Exit", BAct::W_EXIT, 18, sf::Color(100, 30, 60), sf::Color(70, 10, 40));

    win.draw(mkT("v1.0  |  BCS-2G  Spring 2026", 12, P::PRIL(), 900, 700));
}

// ─── LOGIN ───────────────────────────────────────────────────────────────────
void GUI::rLogin(const char* role) {
    sf::RectangleShape hero({ 1280.f, 180.f });
    hero.setFillColor(P::PRI()); win.draw(hero);
    for (int i = 0; i < 7; i++) {
        sf::CircleShape c(50.f + i * 16.f);
        c.setFillColor(sf::Color(255, 255, 255, 10));
        c.setPosition((float)(i * 185 - 50), -25.f);
        win.draw(c);
    }
    sf::Text tt = mkT("MediCore", 32, P::WHITE(), 0, 28);
    tt.setStyle(sf::Text::Bold); ctrT(tt, 640, 28); win.draw(tt);
    sf::Text rt = mkT(role, 18, P::PRIL(), 0, 80);
    ctrT(rt, 640, 80); win.draw(rt);

    // Login card  (y 200..550, height 350)
    drawCard(350, 200, 580, 350);

    // Title inside card
    sf::Text ct = mkT("Sign In", 20, P::PRID(), 0, 222);
    ct.setStyle(sf::Text::Bold);
    ctrT(ct, 640, 222);
    win.draw(ct);

    // Labels — sit just above their boxes (boxes at y=298 and y=380)
    // Box 0 at y=298, label at y=278
    win.draw(mkT("ID", 13, P::TXTM(), 370, 278));
    // Box 1 at y=380, label at y=360
    win.draw(mkT("Password", 13, P::TXTM(), 370, 360));
    drawBoxes();

    // Login button below boxes (boxes end at 380+44=424)
    addBtn(440, 448, 400, 46, "Login", BAct::L_LOGIN, 18, P::PRI(), P::PRID());

    // Back button (special pink-light styling)
    if (nFBtns < MAX_BTNS) {
        Btn& b = fBtns[nFBtns++];
        b.cn = sf::Color(245, 220, 235); b.ch = P::PRIL(); b.act = BAct::L_BACK;
        b.body.setPosition(440, 504); b.body.setSize({ 400, 36 });
        sf::Vector2i pixelPos = sf::Mouse::getPosition(win);
        sf::Vector2f mp = win.mapPixelToCoords(pixelPos);
        b.hov = b.hit(mp);
        b.body.setFillColor(b.hov ? b.ch : b.cn);
        b.lbl = mkT("Back", 14, P::PRID(), 0, 0);
        sf::FloatRect tb = b.lbl.getLocalBounds();
        b.lbl.setPosition(440 + (400 - tb.width) / 2.f - tb.left,
            504 + (36 - tb.height) / 2.f - tb.top - 2.f);
        win.draw(b.body); win.draw(b.lbl);
    }

    // Help card
    drawCard(350, 565, 580, 80);
    win.draw(mkT("Default Credentials:", 12, P::PRID(), 370, 578));
    win.draw(mkT("Patient: 1 / pass123     Doctor: 1 / doc456     Admin: 1 / admin123",
        12, P::TXTM(), 370, 600));
    win.draw(mkT("Press Tab to switch field.   Press Enter to login.",
        11, P::TXTM(), 370, 622));
}

// ─── PATIENT MENU ────────────────────────────────────────────────────────────
void GUI::rPatMenu() {
    banner("Patient Dashboard", curPat ? curPat->getName() : "");

    // Balance card — positioned BELOW banner, ABOVE buttons. Card spans
    // y=110..150. First button starts at y=160 → no overlap.
    if (curPat) {
        drawCard(880, 110, 360, 44);
        char bb[32]; Person::floatToStr(curPat->getBalance(), bb, 32, 2);
        char bs[80] = "Balance: PKR ";
        Person::copyStr(bs + 13, bb, 67);
        sf::Text bt = mkT(bs, 17, P::PRID(), 0, 122);
        bt.setStyle(sf::Text::Bold);
        sf::FloatRect br = bt.getLocalBounds();
        bt.setPosition(880 + (360 - br.width) / 2.f - br.left, 122);
        win.draw(bt);
    }

    float bx = 240, bw = 800, bh = 50;
    addBtn(bx, 160, bw, bh, "1.   Book Appointment", BAct::PM_BOOK, 17, P::PRI(), P::PRID());
    addBtn(bx, 220, bw, bh, "2.   Cancel Appointment", BAct::PM_CANCEL, 17, sf::Color(195, 60, 120), sf::Color(155, 30, 90));
    addBtn(bx, 280, bw, bh, "3.   View My Appointments", BAct::PM_VIEW_APTS, 17, sf::Color(185, 55, 115), sf::Color(145, 25, 85));
    addBtn(bx, 340, bw, bh, "4.   View My Medical Records", BAct::PM_VIEW_REC, 17, sf::Color(175, 50, 110), sf::Color(135, 20, 80));
    addBtn(bx, 400, bw, bh, "5.   View My Bills", BAct::PM_VIEW_BILLS, 17, sf::Color(165, 45, 105), sf::Color(125, 15, 75));
    addBtn(bx, 460, bw, bh, "6.   Pay Bill", BAct::PM_PAY, 17, sf::Color(155, 40, 100), sf::Color(115, 10, 70));
    addBtn(bx, 520, bw, bh, "7.   Top Up Balance", BAct::PM_TOPUP, 17, sf::Color(140, 35, 90), sf::Color(100, 8, 62));
    addBtn(bx, 580, bw, bh, "8.   Logout", BAct::PM_LOGOUT, 17, sf::Color(90, 25, 58), sf::Color(65, 5, 40));
}

// ─── DOCTOR MENU ─────────────────────────────────────────────────────────────
void GUI::rDocMenu() {
    char sub[120] = "";
    if (curDoc) {
        Person::copyStr(sub, "Dr. ", 120);
        int sl = Person::strLen(sub);
        Person::copyStr(sub + sl, curDoc->getName(), 116 - sl);
        sl = Person::strLen(sub);
        Person::copyStr(sub + sl, "  |  ", 120 - sl);
        sl = Person::strLen(sub);
        Person::copyStr(sub + sl, curDoc->getSpecialization(), 120 - sl);
    }
    banner("Doctor Dashboard", sub);

    float bx = 240, bw = 800, bh = 50;
    addBtn(bx, 148, bw, bh, "1.   View Today's Appointments", BAct::DM_TODAY, 17, P::PRI(), P::PRID());
    addBtn(bx, 210, bw, bh, "2.   Mark Appointment Complete", BAct::DM_DONE, 17, sf::Color(185, 55, 115), sf::Color(145, 25, 85));
    addBtn(bx, 272, bw, bh, "3.   Mark Appointment No-Show", BAct::DM_NS, 17, sf::Color(175, 50, 110), sf::Color(135, 20, 80));
    addBtn(bx, 334, bw, bh, "4.   Write Prescription", BAct::DM_PRESC, 17, sf::Color(165, 45, 105), sf::Color(125, 15, 75));
    addBtn(bx, 396, bw, bh, "5.   View Patient Medical History", BAct::DM_HIST, 17, sf::Color(155, 40, 100), sf::Color(115, 10, 70));
    addBtn(bx, 458, bw, bh, "6.   Logout", BAct::DM_LOGOUT, 17, sf::Color(90, 25, 58), sf::Color(65, 5, 40));
}

// ─── ADMIN MENU ──────────────────────────────────────────────────────────────
void GUI::rAdmMenu() {
    banner("Admin Panel", "MediCore Administration");
    float c1 = 50, c2 = 670, bw = 580, bh = 50;
    addBtn(c1, 148, bw, bh, "1.   Add Doctor", BAct::AM_ADD, 17, P::PRI(), P::PRID());
    addBtn(c1, 210, bw, bh, "2.   Remove Doctor", BAct::AM_REM, 17, sf::Color(190, 60, 120), sf::Color(150, 30, 90));
    addBtn(c1, 272, bw, bh, "3.   View All Patients", BAct::AM_PAT, 17, sf::Color(180, 55, 115), sf::Color(140, 25, 85));
    addBtn(c1, 334, bw, bh, "4.   View All Doctors", BAct::AM_DOC, 17, sf::Color(170, 50, 110), sf::Color(130, 20, 80));
    addBtn(c1, 396, bw, bh, "5.   View All Appointments", BAct::AM_APT, 17, sf::Color(160, 45, 105), sf::Color(120, 15, 75));
    addBtn(c2, 148, bw, bh, "6.   View Unpaid Bills", BAct::AM_UNPAID, 17, P::PRI(), P::PRID());
    addBtn(c2, 210, bw, bh, "7.   Discharge Patient", BAct::AM_DISC, 17, sf::Color(190, 60, 120), sf::Color(150, 30, 90));
    addBtn(c2, 272, bw, bh, "8.   View Security Log", BAct::AM_LOG, 17, sf::Color(180, 55, 115), sf::Color(140, 25, 85));
    addBtn(c2, 334, bw, bh, "9.   Generate Daily Report", BAct::AM_REPORT, 17, sf::Color(170, 50, 110), sf::Color(130, 20, 80));
    addBtn(c2, 396, bw, bh, "10.  Logout", BAct::AM_LOGOUT, 17, sf::Color(90, 25, 58), sf::Color(65, 5, 40));
}

// ─── BOOK APPOINTMENT ────────────────────────────────────────────────────────
void GUI::rBook() {
    banner("Book Appointment", "");
    addBackBtn();

    if (bkStep == 0) {
        drawCard(200, 120, 880, 500);
        win.draw(mkT("Step 1 of 4   Search by Specialization", 15, P::TXTM(), 240, 138));
        for (int i = 0; i < 4; i++) {
            sf::CircleShape d(8); d.setFillColor(i == 0 ? P::PRI() : P::PRIL());
            d.setPosition(600.f + i * 50.f, 136.f); win.draw(d);
        }
        win.draw(mkT("Enter the medical specialization you need:", 15, P::TXT(), 240, 305));
        drawBoxes();
        addBtn(440, 392, 360, 48, "Search Doctors", BAct::SUBMIT, 17, P::PRI(), P::PRID());
    }
    else if (bkStep == 1) {
        win.draw(mkT("Step 2 of 4   Choose a Doctor", 15, P::TXTM(), 50, 118));
        for (int i = 0; i < 4; i++) {
            sf::CircleShape d(8); d.setFillColor(i == 1 ? P::PRI() : P::PRIL());
            d.setPosition(600.f + i * 50.f, 116.f); win.draw(d);
        }
        const char* hdr[] = { "ID", "Name", "Specialization", "Fee (PKR)" };
        float hw[] = { 70, 340, 280, 200 };
        tblH(hdr, hw, 4, 50, 148);
        int row = 0;
        for (int i = 0; i < DOC.size(); i++) {
            Doctor& d = DOC.get(i);
            if (Person::compareStrCI(d.getSpecialization(), bkSpec) != 0) continue;
            char idB[10], fB[20];
            Person::intToStr(d.getId(), idB, 10);
            Person::floatToStr(d.getFee(), fB, 20, 2);
            const char* v[] = { idB, d.getName(), d.getSpecialization(), fB };
            float fw[] = { 70, 340, 280, 200 };
            tblR(v, fw, 4, 50, 182.f + row * 30.f, row % 2 == 0);
            ++row;
        }
        if (row == 0) win.draw(mkT("No doctors found.", 16, P::RED(), 50, 185));
        // Label above box (box at y=460, so label at y=435)
        win.draw(mkT("Enter Doctor ID:", 15, P::TXT(), 390, 435));
        drawBoxes();
        // Submit below box (box ends at 460+44=504), button at y=525
        addBtn(440, 525, 360, 48, "Select Doctor", BAct::SUBMIT, 17, P::PRI(), P::PRID());
    }
    else if (bkStep == 2) {
        win.draw(mkT("Step 3 of 4   Choose a Date", 15, P::TXTM(), 50, 118));
        for (int i = 0; i < 4; i++) {
            sf::CircleShape d(8); d.setFillColor(i == 2 ? P::PRI() : P::PRIL());
            d.setPosition(600.f + i * 50.f, 116.f); win.draw(d);
        }
        Doctor* d = DOC.findById(bkDocId);
        if (d) {
            drawCard(50, 145, 1180, 80);
            char fB[20]; Person::floatToStr(d->getFee(), fB, 20, 2);
            char info[120] = "Selected: ";
            Person::copyStr(info + 10, d->getName(), 80);
            int il = Person::strLen(info);
            Person::copyStr(info + il, "  |  Fee: PKR ", 50);
            il = Person::strLen(info);
            Person::copyStr(info + il, fB, 30);
            win.draw(mkT(info, 15, P::PRID(), 70, 168));
            char binfo[80] = "Your Balance: PKR ";
            char bb[20]; Person::floatToStr(curPat->getBalance(), bb, 20, 2);
            Person::copyStr(binfo + Person::strLen(binfo), bb, 62);
            win.draw(mkT(binfo, 14, P::TXTM(), 70, 196));
        }
        // Label above box (box at y=320, so label at y=295)
        win.draw(mkT("Enter appointment date (DD-MM-YYYY):", 15, P::TXT(), 390, 295));
        drawBoxes();
        // Submit below box (box ends at 320+44=364), button at y=395
        addBtn(440, 395, 360, 48, "Check Available Slots", BAct::SUBMIT, 16, P::PRI(), P::PRID());
    }
    else if (bkStep == 3) {
        win.draw(mkT("Step 4 of 4   Choose a Time Slot", 15, P::TXTM(), 50, 118));
        for (int i = 0; i < 4; i++) {
            sf::CircleShape d(8); d.setFillColor(i == 3 ? P::PRI() : P::PRIL());
            d.setPosition(600.f + i * 50.f, 116.f); win.draw(d);
        }
        char dl[40] = "Date: ";
        Person::copyStr(dl + 6, bkDate, 34);
        win.draw(mkT(dl, 14, P::TXTM(), 50, 148));

        // Slot grid: 8 buttons in 2 rows of 4 (y=172..290)
        const char* slots[] = { "09:00","10:00","11:00","12:00","13:00","14:00","15:00","16:00" };
        BAct slotActs[] = { BAct::SLOT_0,BAct::SLOT_1,BAct::SLOT_2,BAct::SLOT_3,
                           BAct::SLOT_4,BAct::SLOT_5,BAct::SLOT_6,BAct::SLOT_7 };
        for (int s = 0; s < 8; s++) {
            float sx = 50.f + (s % 4) * 295.f;
            float sy = 172.f + (s / 4) * 66.f;
            bool free = slotFree(bkDocId, bkDate, slots[s]);
            bool sel = (Person::compareStr(bkSlot, slots[s]) == 0);
            sf::Color bg = sel ? P::PRID() : (free ? P::GREEN() : sf::Color(185, 185, 185));
            sf::Color hov = sel ? P::PRID() : (free ? P::GREEND() : sf::Color(165, 165, 165));
            addBtn(sx, sy, 265.f, 52.f, slots[s], slotActs[s], 17, bg, hov);
            win.draw(mkT(free ? (sel ? "Selected" : "Available") : "Taken",
                11, sel ? P::WHITE() : (free ? P::WHITE() : sf::Color(100, 100, 100)),
                sx + 90, sy + 36));
        }

        // Fee/balance info row (between slots and input)
        Doctor* d = DOC.findById(bkDocId);
        if (d && curPat) {
            char fB[20], bB[20];
            Person::floatToStr(d->getFee(), fB, 20, 2);
            Person::floatToStr(curPat->getBalance(), bB, 20, 2);
            char info[120] = "Fee: PKR ";
            Person::copyStr(info + 9, fB, 60);
            int il = Person::strLen(info);
            Person::copyStr(info + il, "     Balance: PKR ", 20);
            il = Person::strLen(info);
            Person::copyStr(info + il, bB, 40);
            sf::Color fc = (curPat->getBalance() >= d->getFee()) ? P::GREEN() : P::RED();
            sf::Text it = mkT(info, 14, fc, 0, 320);
            ctrT(it, 640, 320);
            win.draw(it);
        }

        // Label above input box (box at y=380)
        win.draw(mkT("Selected slot / Type slot:", 14, P::TXTM(), 390, 358));
        drawBoxes();

        // Confirm button below box (box ends at 380+44=424)
        addBtn(440, 450, 360, 48, "Confirm Booking", BAct::SUBMIT, 17, P::PRI(), P::PRID());
    }
}

// ─── CANCEL APPOINTMENT ──────────────────────────────────────────────────────
void GUI::rCancelApt() {
    banner("Cancel Appointment", "");
    addBackBtn();
    const char* hdr[] = { "Appt ID","Doctor","Date","Slot","Status" };
    float hw[] = { 100, 340, 150, 100, 150 };
    tblH(hdr, hw, 5, 50, 118);
    int row = 0;
    for (int i = 0; i < APT.size(); i++) {
        Appointment& a = APT.get(i);
        if (a.getPatientId() != curPat->getId() ||
            Person::compareStr(a.getStatus(), "pending") != 0) continue;
        char id[10]; Person::intToStr(a.getAppointmentId(), id, 10);
        Doctor* d = DOC.findById(a.getDoctorId());
        char dn[100] = "?"; if (d) Person::copyStr(dn, d->getName(), 100);
        const char* v[] = { id, dn, a.getDate(), a.getTimeSlot(), "pending" };
        float fw[] = { 100, 340, 150, 100, 150 };
        tblR(v, fw, 5, 50, 152.f + row * 30.f, row % 2 == 0);
        ++row;
    }
    if (row == 0) win.draw(mkT("No pending appointments.", 17, P::TXTM(), 50, 160));
    sf::RectangleShape div2({ 1180.f, 1.f });
    div2.setPosition(50, 470); div2.setFillColor(P::PRIL()); win.draw(div2);
    win.draw(mkT("Appointment ID to cancel:", 15, P::TXT(), 390, 482));
    drawBoxes();
    addBtn(440, 568, 360, 46, "Cancel Appointment", BAct::SUBMIT, 16, P::RED(), P::REDD());
}

// ─── VIEW APPOINTMENTS ───────────────────────────────────────────────────────
void GUI::rViewApts() {
    banner("My Appointments", "Sorted by date ascending");
    addBackBtn();
    Appointment arr[100]; int cnt = 0;
    for (int i = 0; i < APT.size(); i++)
        if (APT.get(i).getPatientId() == curPat->getId()) arr[cnt++] = APT.get(i);
    sortAptAsc(arr, cnt);
    if (cnt == 0) { win.draw(mkT("No appointments found.", 18, P::TXTM(), 50, 160)); return; }
    const char* hdr[] = { "ID","Doctor","Specialization","Date","Slot","Status" };
    float hw[] = { 60, 220, 210, 140, 90, 140 };
    tblH(hdr, hw, 6, 50, 118);
    float sy = 152.f - scroll;
    for (int i = 0; i < cnt; i++) {
        float ry = sy + i * 30.f;
        if (ry < 118 || ry > 676) continue;
        char id[10]; Person::intToStr(arr[i].getAppointmentId(), id, 10);
        Doctor* d = DOC.findById(arr[i].getDoctorId());
        char dn[100] = "?", ds[60] = "?";
        if (d) {
            Person::copyStr(dn, d->getName(), 100);
            Person::copyStr(ds, d->getSpecialization(), 60);
        }
        const char* v[] = { id, dn, ds, arr[i].getDate(), arr[i].getTimeSlot(), arr[i].getStatus() };
        float fw[] = { 60, 220, 210, 140, 90, 140 };
        tblR(v, fw, 6, 50, ry, i % 2 == 0);
    }
    win.draw(mkT("Mouse wheel to scroll", 12, P::TXTM(), 50, 695));
}

// ─── VIEW MEDICAL RECORDS ────────────────────────────────────────────────────
void GUI::rViewRec() {
    banner("Medical Records", "Most recent first");
    addBackBtn();
    Prescription arr[100]; int cnt = 0;
    for (int i = 0; i < PRE.size(); i++)
        if (PRE.get(i).getPatientId() == curPat->getId()) arr[cnt++] = PRE.get(i);
    sortPreDesc(arr, cnt);
    if (cnt == 0) { win.draw(mkT("No medical records found.", 18, P::TXTM(), 50, 160)); return; }
    float y = 118.f - scroll;
    for (int i = 0; i < cnt; i++) {
        float cy = y + i * 148.f;
        if (cy + 148 < 118 || cy > 676) continue;
        drawCard(50, cy, 1180, 138);
        Doctor* d = DOC.findById(arr[i].getDoctorId());
        char dn[100] = "Unknown"; if (d) Person::copyStr(dn, d->getName(), 100);
        char dl[60] = "Date: "; Person::copyStr(dl + 6, arr[i].getDate(), 54);
        char dcl[120] = "Doctor: "; Person::copyStr(dcl + 8, dn, 112);
        win.draw(mkT(dl, 14, P::TXTM(), 70, cy + 10));
        win.draw(mkT(dcl, 14, P::PRID(), 400, cy + 10));
        sf::RectangleShape sep({ 1100.f, 1.f });
        sep.setPosition(70, cy + 34); sep.setFillColor(P::PRIL()); win.draw(sep);
        char ml[520] = "Medicines: "; Person::copyStr(ml + 11, arr[i].getMedicines(), 509);
        char nl[320] = "Notes: ";     Person::copyStr(nl + 7, arr[i].getNotes(), 313);
        win.draw(mkT(ml, 13, P::TXT(), 70, cy + 44));
        win.draw(mkT(nl, 13, P::TXT(), 70, cy + 76));
    }
    win.draw(mkT("Mouse wheel to scroll", 12, P::TXTM(), 50, 695));
}

// ─── VIEW BILLS ──────────────────────────────────────────────────────────────
void GUI::rViewBills() {
    banner("My Bills", "");
    addBackBtn();
    const char* hdr[] = { "Bill ID","Appt ID","Amount (PKR)","Status","Date" };
    float hw[] = { 100, 120, 210, 160, 200 };
    tblH(hdr, hw, 5, 50, 118);
    float total = 0; int row = 0; float sy = 152.f - scroll;
    for (int i = 0; i < BIL.size(); i++) {
        Bill& b = BIL.get(i);
        if (b.getPatientId() != curPat->getId()) continue;
        if (Person::compareStr(b.getStatus(), "unpaid") == 0) total += b.getAmount();
        float ry = sy + row * 30.f;
        if (ry < 118 || ry > 665) { ++row; continue; }
        char bi[10], ai[10], am[20];
        Person::intToStr(b.getBillId(), bi, 10);
        Person::intToStr(b.getAppointmentId(), ai, 10);
        Person::floatToStr(b.getAmount(), am, 20, 2);
        const char* v[] = { bi, ai, am, b.getStatus(), b.getDate() };
        float fw[] = { 100, 120, 210, 160, 200 };
        tblR(v, fw, 5, 50, ry, row % 2 == 0);
        ++row;
    }
    if (row == 0) win.draw(mkT("No bills found.", 18, P::TXTM(), 50, 160));
    sf::RectangleShape ft({ 1180.f, 34.f });
    ft.setPosition(50, 666); ft.setFillColor(P::PRIEL()); win.draw(ft);
    char tb[32]; Person::floatToStr(total, tb, 32, 2);
    char ts[80] = "Total Outstanding (Unpaid): PKR ";
    Person::copyStr(ts + Person::strLen(ts), tb, 48);
    sf::Text tst = mkT(ts, 15, P::PRID(), 0, 672);
    tst.setStyle(sf::Text::Bold); ctrT(tst, 640, 672); win.draw(tst);
}

// ─── PAY BILL ────────────────────────────────────────────────────────────────
void GUI::rPayBill() {
    banner("Pay Bill", "");
    addBackBtn();
    const char* hdr[] = { "Bill ID","Amount (PKR)","Date","Status" };
    float hw[] = { 120, 220, 220, 160 };
    tblH(hdr, hw, 4, 50, 118);
    int row = 0;
    for (int i = 0; i < BIL.size(); i++) {
        Bill& b = BIL.get(i);
        if (b.getPatientId() != curPat->getId() ||
            Person::compareStr(b.getStatus(), "unpaid") != 0) continue;
        char bi[10], am[20];
        Person::intToStr(b.getBillId(), bi, 10);
        Person::floatToStr(b.getAmount(), am, 20, 2);
        const char* v[] = { bi, am, b.getDate(), "unpaid" };
        float fw[] = { 120, 220, 220, 160 };
        tblR(v, fw, 4, 50, 152.f + row * 30.f, row % 2 == 0);
        ++row;
    }
    if (row == 0) win.draw(mkT("No unpaid bills.", 18, P::TXTM(), 50, 160));
    sf::RectangleShape div2({ 1180.f, 1.f });
    div2.setPosition(50, 455); div2.setFillColor(P::PRIL()); win.draw(div2);
    win.draw(mkT("Enter Bill ID to pay:", 15, P::TXT(), 390, 482));
    drawBoxes();
    addBtn(440, 568, 360, 46, "Pay Bill", BAct::SUBMIT, 18, P::PRI(), P::PRID());
}

// ─── TOP UP ──────────────────────────────────────────────────────────────────
void GUI::rTopup() {
    banner("Top Up Balance", "");
    addBackBtn();
    drawCard(390, 165, 500, 310);
    char bb[32]; Person::floatToStr(curPat->getBalance(), bb, 32, 2);
    win.draw(mkT("Current Balance", 16, P::TXTM(), 440, 188));
    char balShow[60] = "PKR "; Person::copyStr(balShow + 4, bb, 56);
    sf::Text bst = mkT(balShow, 30, P::PRID(), 0, 218);
    bst.setStyle(sf::Text::Bold); ctrT(bst, 640, 218); win.draw(bst);
    sf::RectangleShape sep({ 420.f, 1.f });
    sep.setPosition(440, 272); sep.setFillColor(P::PRIL()); win.draw(sep);
    win.draw(mkT("Amount to add (PKR):", 15, P::TXT(), 440, 286));
    drawBoxes();
    addBtn(440, 405, 360, 46, "Top Up", BAct::SUBMIT, 18, P::PRI(), P::PRID());
}

// ─── TODAY'S APPOINTMENTS ────────────────────────────────────────────────────
void GUI::rToday() {
    char today[11]; FileHandler::getTodayDate(today);
    char title[60] = "Today's Appointments - ";
    Person::copyStr(title + Person::strLen(title), today, 36);
    banner(title, "");
    addBackBtn();
    const char* hdr[] = { "Appt ID","Patient Name","Time Slot","Status" };
    float hw[] = { 110, 420, 140, 190 };
    tblH(hdr, hw, 4, 50, 118);
    Appointment arr[100]; int cnt = 0;
    for (int i = 0; i < APT.size(); i++) {
        Appointment& a = APT.get(i);
        if (a.getDoctorId() == curDoc->getId() && Person::compareStr(a.getDate(), today) == 0)
            arr[cnt++] = a;
    }
    for (int i = 0; i < cnt - 1; i++) for (int j = 0; j < cnt - i - 1; j++)
        if (Appointment::compareSlots(arr[j].getTimeSlot(), arr[j + 1].getTimeSlot()) > 0) {
            Appointment t = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = t;
        }
    if (cnt == 0) {
        win.draw(mkT("No appointments scheduled for today.", 18, P::TXTM(), 50, 160));
        return;
    }
    for (int i = 0; i < cnt; i++) {
        char id[10]; Person::intToStr(arr[i].getAppointmentId(), id, 10);
        Patient* p = PAT.findById(arr[i].getPatientId());
        char pn[100] = "?"; if (p) Person::copyStr(pn, p->getName(), 100);
        const char* v[] = { id, pn, arr[i].getTimeSlot(), arr[i].getStatus() };
        float fw[] = { 110, 420, 140, 190 };
        tblR(v, fw, 4, 50, 152.f + i * 30.f, i % 2 == 0);
    }
}

// ─── MARK DONE ───────────────────────────────────────────────────────────────
void GUI::rMarkDone() {
    banner("Mark Appointment Complete", "");
    addBackBtn();
    char today[11]; FileHandler::getTodayDate(today);
    const char* hdr[] = { "Appt ID","Patient","Slot" };
    float hw[] = { 130, 560, 200 };
    tblH(hdr, hw, 3, 50, 118);
    int row = 0;
    for (int i = 0; i < APT.size(); i++) {
        Appointment& a = APT.get(i);
        if (a.getDoctorId() != curDoc->getId() ||
            Person::compareStr(a.getDate(), today) != 0 ||
            Person::compareStr(a.getStatus(), "pending") != 0) continue;
        char id[10]; Person::intToStr(a.getAppointmentId(), id, 10);
        Patient* p = PAT.findById(a.getPatientId());
        char pn[100] = "?"; if (p) Person::copyStr(pn, p->getName(), 100);
        const char* v[] = { id, pn, a.getTimeSlot() };
        float fw[] = { 130, 560, 200 };
        tblR(v, fw, 3, 50, 152.f + row * 30.f, row % 2 == 0);
        ++row;
    }
    if (row == 0) win.draw(mkT("No pending appointments for today.", 17, P::TXTM(), 50, 160));
    win.draw(mkT("Enter Appointment ID:", 15, P::TXT(), 440, 437));
    drawBoxes();
    addBtn(440, 520, 360, 46, "Mark as Completed", BAct::SUBMIT, 16, P::GREEN(), P::GREEND());
}

// ─── MARK NO-SHOW ────────────────────────────────────────────────────────────
void GUI::rMarkNS() {
    banner("Mark Appointment No-Show", "");
    addBackBtn();
    char today[11]; FileHandler::getTodayDate(today);
    const char* hdr[] = { "Appt ID","Patient","Slot" };
    float hw[] = { 130, 560, 200 };
    tblH(hdr, hw, 3, 50, 118);
    int row = 0;
    for (int i = 0; i < APT.size(); i++) {
        Appointment& a = APT.get(i);
        if (a.getDoctorId() != curDoc->getId() ||
            Person::compareStr(a.getDate(), today) != 0 ||
            Person::compareStr(a.getStatus(), "pending") != 0) continue;
        char id[10]; Person::intToStr(a.getAppointmentId(), id, 10);
        Patient* p = PAT.findById(a.getPatientId());
        char pn[100] = "?"; if (p) Person::copyStr(pn, p->getName(), 100);
        const char* v[] = { id, pn, a.getTimeSlot() };
        float fw[] = { 130, 560, 200 };
        tblR(v, fw, 3, 50, 152.f + row * 30.f, row % 2 == 0);
        ++row;
    }
    if (row == 0) win.draw(mkT("No pending appointments for today.", 17, P::TXTM(), 50, 160));
    win.draw(mkT("Enter Appointment ID:", 15, P::TXT(), 440, 437));
    drawBoxes();
    addBtn(440, 520, 360, 46, "Mark as No-Show", BAct::SUBMIT, 16, P::RED(), P::REDD());
}

// ─── WRITE PRESCRIPTION ──────────────────────────────────────────────────────
void GUI::rWritePresc() {
    banner("Write Prescription", "");
    addBackBtn();
    drawCard(200, 118, 880, 520);
    // Step indicator dots (Appt → Meds → Notes)
    for (int i = 0; i < 3; i++) {
        sf::CircleShape d(12);
        d.setFillColor(i <= prStep ? P::PRI() : P::PRIL());
        d.setPosition(590.f + i * 60.f, 128.f);
        win.draw(d);
        const char* lbl[] = { "Appt","Meds","Notes" };
        win.draw(mkT(lbl[i], 11, i <= prStep ? P::PRID() : P::TXTM(),
            585.f + i * 60.f, 152.f));
    }
    // Box 0 at y=270, Box 1 at y=370, Box 2 at y=470
    // Labels sit 22px above each box
    win.draw(mkT("Appointment ID (completed only):", 14, P::TXTM(), 220, 248));
    if (prStep >= 1) win.draw(mkT("Medicines (semicolon-separated):", 14, P::TXTM(), 220, 348));
    if (prStep >= 2) win.draw(mkT("Notes:", 14, P::TXTM(), 220, 448));
    drawBoxes();
    addBtn(440, 578, 360, 46, "Save Prescription", BAct::SUBMIT, 16, P::PRI(), P::PRID());
}

// ─── PATIENT HISTORY ─────────────────────────────────────────────────────────
void GUI::rPatHist() {
    banner("Patient Medical History", "");
    addBackBtn();
    win.draw(mkT("Enter Patient ID:", 15, P::TXT(), 440, 128));
    drawBoxes();
    addBtn(440, 220, 360, 44, "View History", BAct::NONE, 16, P::PRI(), P::PRID());

    int pid = Person::strToInt(boxes[0].get());
    if (pid <= 0) return;
    if (!PAT.findById(pid)) {
        win.draw(mkT("Patient not found.", 16, P::RED(), 50, 282));
        return;
    }
    bool ok = false;
    for (int i = 0; i < APT.size(); i++) {
        Appointment& a = APT.get(i);
        if (a.getPatientId() == pid && a.getDoctorId() == curDoc->getId() &&
            Person::compareStr(a.getStatus(), "completed") == 0) {
            ok = true; break;
        }
    }
    if (!ok) {
        win.draw(mkT("Access denied. No completed appointments with this patient.",
            15, P::RED(), 50, 282));
        return;
    }
    Prescription arr[100]; int cnt = 0;
    for (int i = 0; i < PRE.size(); i++) {
        Prescription& pr = PRE.get(i);
        if (pr.getPatientId() == pid && pr.getDoctorId() == curDoc->getId())
            arr[cnt++] = pr;
    }
    sortPreDesc(arr, cnt);
    if (cnt == 0) {
        win.draw(mkT("No prescriptions written for this patient.", 16, P::TXTM(), 50, 282));
        return;
    }
    float y = 282.f - scroll;
    for (int i = 0; i < cnt; i++) {
        float cy = y + i * 130.f;
        if (cy + 130 < 270 || cy > 676) continue;
        drawCard(50, cy, 1180, 120);
        char dl[60] = "Date: "; Person::copyStr(dl + 6, arr[i].getDate(), 54);
        char ml[520] = "Medicines: "; Person::copyStr(ml + 11, arr[i].getMedicines(), 509);
        char nl[320] = "Notes: "; Person::copyStr(nl + 7, arr[i].getNotes(), 313);
        win.draw(mkT(dl, 14, P::TXTM(), 70, cy + 8));
        win.draw(mkT(ml, 13, P::TXT(), 70, cy + 36));
        win.draw(mkT(nl, 13, P::TXT(), 70, cy + 66));
    }
}

// ─── ADD DOCTOR ──────────────────────────────────────────────────────────────
void GUI::rAddDoc() {
    banner("Add New Doctor", "");
    addBackBtn();
    drawCard(300, 130, 680, 410);
    const char* lbx[] = { "Doctor Name:","Specialization:","Contact (11 digits):",
                         "Password (min 6 chars):","Consultation Fee (PKR):" };
    // Each label sits 22px above its box (boxes at 178, 248, 318, 388, 458)
    float labelYs[] = { 156, 226, 296, 366, 436 };
    for (int i = 0; i < 5; i++) win.draw(mkT(lbx[i], 13, P::TXTM(), 320, labelYs[i]));
    drawBoxes();
    addBtn(440, 560, 360, 46, "Add Doctor", BAct::SUBMIT, 18, P::PRI(), P::PRID());
}

// ─── REMOVE DOCTOR ───────────────────────────────────────────────────────────
void GUI::rRemDoc() {
    banner("Remove Doctor", "");
    addBackBtn();
    const char* hdr[] = { "ID","Name","Specialization","Fee (PKR)" };
    float hw[] = { 80, 340, 280, 200 };
    tblH(hdr, hw, 4, 50, 118);
    int row = 0;
    for (int i = 0; i < DOC.size(); i++) {
        Doctor& d = DOC.get(i);
        char id[10], fe[20];
        Person::intToStr(d.getId(), id, 10);
        Person::floatToStr(d.getFee(), fe, 20, 2);
        const char* v[] = { id, d.getName(), d.getSpecialization(), fe };
        float fw[] = { 80, 340, 280, 200 };
        tblR(v, fw, 4, 50, 152.f + row * 30.f, row % 2 == 0);
        ++row;
    }
    if (row == 0) win.draw(mkT("No doctors found.", 17, P::TXTM(), 50, 160));
    sf::RectangleShape div2({ 1180.f, 1.f });
    div2.setPosition(50, 455); div2.setFillColor(P::PRIL()); win.draw(div2);
    win.draw(mkT("Enter Doctor ID to remove:", 15, P::TXT(), 390, 482));
    drawBoxes();
    addBtn(440, 568, 360, 46, "Remove Doctor", BAct::SUBMIT, 16, P::RED(), P::REDD());
}

// ─── ALL PATIENTS ────────────────────────────────────────────────────────────
void GUI::rAllPat() {
    banner("All Patients", "");
    addBackBtn();
    const char* hdr[] = { "ID","Name","Age","Gender","Contact","Balance","Unpaid" };
    float hw[] = { 60, 240, 60, 80, 150, 150, 120 };
    tblH(hdr, hw, 7, 50, 118);
    int row = 0; float sy = 152.f - scroll;
    for (int i = 0; i < PAT.size(); i++) {
        Patient& p = PAT.get(i);
        int un = 0;
        for (int j = 0; j < BIL.size(); j++)
            if (BIL.get(j).getPatientId() == p.getId() &&
                Person::compareStr(BIL.get(j).getStatus(), "unpaid") == 0) ++un;
        char id[10], ag[10], ba[20], ub[10];
        Person::intToStr(p.getId(), id, 10);
        Person::intToStr(p.getAge(), ag, 10);
        Person::floatToStr(p.getBalance(), ba, 20, 2);
        Person::intToStr(un, ub, 10);
        float ry = sy + row * 30.f;
        if (ry < 118 || ry > 676) { ++row; continue; }
        const char* v[] = { id, p.getName(), ag, p.getGender(), p.getContact(), ba, ub };
        float fw[] = { 60, 240, 60, 80, 150, 150, 120 };
        tblR(v, fw, 7, 50, ry, row % 2 == 0);
        ++row;
    }
    if (row == 0) win.draw(mkT("No patients found.", 18, P::TXTM(), 50, 160));
    win.draw(mkT("Mouse wheel to scroll", 12, P::TXTM(), 50, 695));
}

// ─── ALL DOCTORS ─────────────────────────────────────────────────────────────
void GUI::rAllDoc() {
    banner("All Doctors", "");
    addBackBtn();
    const char* hdr[] = { "ID","Name","Specialization","Contact","Fee (PKR)" };
    float hw[] = { 70, 280, 260, 200, 200 };
    tblH(hdr, hw, 5, 50, 118);
    int row = 0;
    for (int i = 0; i < DOC.size(); i++) {
        Doctor& d = DOC.get(i);
        char id[10], fe[20];
        Person::intToStr(d.getId(), id, 10);
        Person::floatToStr(d.getFee(), fe, 20, 2);
        const char* v[] = { id, d.getName(), d.getSpecialization(), d.getContact(), fe };
        float fw[] = { 70, 280, 260, 200, 200 };
        tblR(v, fw, 5, 50, 152.f + row * 30.f, row % 2 == 0);
        ++row;
    }
    if (row == 0) win.draw(mkT("No doctors found.", 18, P::TXTM(), 50, 160));
}

// ─── ALL APPOINTMENTS ────────────────────────────────────────────────────────
void GUI::rAllApt() {
    banner("All Appointments", "Sorted by date descending");
    addBackBtn();
    int n = APT.size();
    Appointment* arr = new Appointment[n < 1 ? 1 : n];
    for (int i = 0; i < n; i++) arr[i] = APT.get(i);
    sortAptDesc(arr, n);
    const char* hdr[] = { "ID","Patient","Doctor","Date","Slot","Status" };
    float hw[] = { 60, 230, 230, 140, 90, 130 };
    tblH(hdr, hw, 6, 50, 118);
    float sy = 152.f - scroll;
    for (int i = 0; i < n; i++) {
        float ry = sy + i * 30.f;
        if (ry < 118 || ry > 676) continue;
        char id[10]; Person::intToStr(arr[i].getAppointmentId(), id, 10);
        Patient* p = PAT.findById(arr[i].getPatientId());
        Doctor* d = DOC.findById(arr[i].getDoctorId());
        char pn[100] = "?", dn[100] = "?";
        if (p) Person::copyStr(pn, p->getName(), 100);
        if (d) Person::copyStr(dn, d->getName(), 100);
        const char* v[] = { id, pn, dn, arr[i].getDate(), arr[i].getTimeSlot(), arr[i].getStatus() };
        float fw[] = { 60, 230, 230, 140, 90, 130 };
        tblR(v, fw, 6, 50, ry, i % 2 == 0);
    }
    if (n == 0) win.draw(mkT("No appointments found.", 18, P::TXTM(), 50, 160));
    delete[] arr;
    win.draw(mkT("Mouse wheel to scroll", 12, P::TXTM(), 50, 695));
}

// ─── UNPAID BILLS ────────────────────────────────────────────────────────────
void GUI::rUnpaid() {
    banner("Unpaid Bills", "Bills older than 7 days are marked OVERDUE");
    addBackBtn();
    const char* hdr[] = { "Bill ID","Patient Name","Amount (PKR)","Date" };
    float hw[] = { 110, 420, 220, 300 };
    tblH(hdr, hw, 4, 50, 118);
    int row = 0;
    for (int i = 0; i < BIL.size(); i++) {
        Bill& b = BIL.get(i);
        if (Person::compareStr(b.getStatus(), "unpaid") != 0) continue;
        Patient* p = PAT.findById(b.getPatientId());
        char pn[100] = "?"; if (p) Person::copyStr(pn, p->getName(), 100);
        char bi[10], am[20];
        Person::intToStr(b.getBillId(), bi, 10);
        Person::floatToStr(b.getAmount(), am, 20, 2);
        const char* bd = b.getDate();
        struct tm t = {};
        t.tm_mday = (bd[0] - '0') * 10 + (bd[1] - '0');
        t.tm_mon = (bd[3] - '0') * 10 + (bd[4] - '0') - 1;
        t.tm_year = (bd[6] - '0') * 1000 + (bd[7] - '0') * 100 + (bd[8] - '0') * 10 + (bd[9] - '0') - 1900;
        time_t bt = mktime(&t);
        int days = (int)(difftime(time(nullptr), bt) / 86400.0);
        char dl[50]; Person::copyStr(dl, b.getDate(), 50);
        if (days > 7) {
            int dl2 = Person::strLen(dl);
            Person::copyStr(dl + dl2, " [OVERDUE]", 50 - dl2);
        }
        const char* v[] = { bi, pn, am, dl };
        float fw[] = { 110, 420, 220, 300 };
        tblR(v, fw, 4, 50, 152.f + row * 30.f, row % 2 == 0);
        ++row;
    }
    if (row == 0) win.draw(mkT("No unpaid bills.", 18, P::TXTM(), 50, 160));
}

// ─── DISCHARGE ───────────────────────────────────────────────────────────────
void GUI::rDischarge() {
    banner("Discharge Patient", "Must have no unpaid bills or pending appointments");
    addBackBtn();
    const char* hdr[] = { "ID","Name","Balance","Unpaid Bills","Pending Appts" };
    float hw[] = { 70, 290, 170, 190, 190 };
    tblH(hdr, hw, 5, 50, 118);
    int row = 0;
    for (int i = 0; i < PAT.size(); i++) {
        Patient& p = PAT.get(i);
        int un = 0, pn = 0;
        for (int j = 0; j < BIL.size(); j++)
            if (BIL.get(j).getPatientId() == p.getId() &&
                Person::compareStr(BIL.get(j).getStatus(), "unpaid") == 0) ++un;
        for (int j = 0; j < APT.size(); j++)
            if (APT.get(j).getPatientId() == p.getId() &&
                Person::compareStr(APT.get(j).getStatus(), "pending") == 0) ++pn;
        char id[10], ba[20], ub[10], pb[10];
        Person::intToStr(p.getId(), id, 10);
        Person::floatToStr(p.getBalance(), ba, 20, 2);
        Person::intToStr(un, ub, 10);
        Person::intToStr(pn, pb, 10);
        const char* v[] = { id, p.getName(), ba, ub, pb };
        float fw[] = { 70, 290, 170, 190, 190 };
        tblR(v, fw, 5, 50, 152.f + row * 30.f, row % 2 == 0);
        ++row;
    }
    sf::RectangleShape div2({ 1180.f, 1.f });
    div2.setPosition(50, 455); div2.setFillColor(P::PRIL()); win.draw(div2);
    win.draw(mkT("Enter Patient ID to discharge:", 15, P::TXT(), 390, 482));
    drawBoxes();
    addBtn(440, 568, 360, 46, "Discharge Patient", BAct::SUBMIT, 16, P::RED(), P::REDD());
}

// ─── SECURITY LOG ────────────────────────────────────────────────────────────
void GUI::rSecLog() {
    banner("Security Log", "");
    addBackBtn();
    FILE* f = fopen("security_log.txt", "r");
    if (!f) { win.draw(mkT("No security events logged.", 18, P::TXTM(), 50, 160)); return; }
    char line[300]; int row = 0; float sy = 118.f - scroll;
    while (fgets(line, 300, f)) {
        float ry = sy + row * 28.f;
        if (ry > 676) break;
        if (ry < 118) { ++row; continue; }
        sf::RectangleShape bg({ 1180.f, 26.f });
        bg.setPosition(50, ry);
        bg.setFillColor(row % 2 == 0 ? P::ROWA() : P::ROWB());
        win.draw(bg);
        int ll = Person::strLen(line);
        if (ll > 0 && (line[ll - 1] == '\n' || line[ll - 1] == '\r')) line[ll - 1] = '\0';
        win.draw(mkT(line, 13, P::TXT(), 58, ry + 5));
        ++row;
    }
    fclose(f);
    if (row == 0) win.draw(mkT("No security events logged.", 18, P::TXTM(), 50, 160));
    win.draw(mkT("Mouse wheel to scroll", 12, P::TXTM(), 50, 695));
}

// ─── DAILY REPORT ────────────────────────────────────────────────────────────
void GUI::rReport() {
    char today[11]; FileHandler::getTodayDate(today);
    char title[60] = "Daily Report - ";
    Person::copyStr(title + Person::strLen(title), today, 44);
    banner(title, "");
    addBackBtn();

    int tot = 0, pend = 0, comp = 0, ns = 0, canc = 0;
    float rev = 0;
    for (int i = 0; i < APT.size(); i++) {
        Appointment& a = APT.get(i);
        if (Person::compareStr(a.getDate(), today) != 0) continue;
        ++tot;
        if (Person::compareStr(a.getStatus(), "pending") == 0)   ++pend;
        else if (Person::compareStr(a.getStatus(), "completed") == 0) ++comp;
        else if (Person::compareStr(a.getStatus(), "noshow") == 0)    ++ns;
        else if (Person::compareStr(a.getStatus(), "cancelled") == 0) ++canc;
    }
    for (int i = 0; i < BIL.size(); i++) {
        Bill& b = BIL.get(i);
        if (Person::compareStr(b.getDate(), today) == 0 &&
            Person::compareStr(b.getStatus(), "paid") == 0)
            rev += b.getAmount();
    }

    struct StatCard { const char* lbl; int val; float x; };
    StatCard sc[] = { {"Total",tot,50},{"Pending",pend,365},{"Completed",comp,680},{"No-Show",ns,995} };
    for (auto& s : sc) {
        drawCard(s.x, 118, 295, 90);
        win.draw(mkT(s.lbl, 14, P::TXTM(), s.x + 20, 132));
        char vb[10]; Person::intToStr(s.val, vb, 10);
        sf::Text vt = mkT(vb, 34, P::PRID(), 0, 150);
        vt.setStyle(sf::Text::Bold);
        ctrT(vt, s.x + 147, 150);
        win.draw(vt);
    }
    drawCard(50, 224, 590, 78);
    char rb[32]; Person::floatToStr(rev, rb, 32, 2);
    char rs[80] = "Revenue Today: PKR ";
    Person::copyStr(rs + Person::strLen(rs), rb, 61);
    sf::Text rt = mkT(rs, 17, P::PRID(), 0, 252);
    rt.setStyle(sf::Text::Bold);
    ctrT(rt, 345, 252);
    win.draw(rt);

    drawCard(660, 224, 570, 78);
    int uc = 0; float ua = 0;
    for (int i = 0; i < BIL.size(); i++)
        if (Person::compareStr(BIL.get(i).getStatus(), "unpaid") == 0) {
            ++uc; ua += BIL.get(i).getAmount();
        }
    char ucb[10], uab[32];
    Person::intToStr(uc, ucb, 10);
    Person::floatToStr(ua, uab, 32, 2);
    char us[80]; Person::copyStr(us, ucb, 80);
    int ul = Person::strLen(us);
    Person::copyStr(us + ul, " patients  PKR ", 80 - ul);
    ul = Person::strLen(us);
    Person::copyStr(us + ul, uab, 80 - ul);
    win.draw(mkT("Outstanding Unpaid Bills:", 14, P::TXTM(), 680, 238));
    win.draw(mkT(us, 15, P::PRID(), 680, 262));

    win.draw(mkT("Doctor-wise Summary for Today:", 16, P::PRID(), 50, 322));
    const char* hdr[] = { "Doctor Name","Completed","Pending","No-Show" };
    float hw[] = { 440, 200, 200, 200 };
    tblH(hdr, hw, 4, 50, 348);
    int dr = 0;
    for (int i = 0; i < DOC.size(); i++) {
        Doctor& d = DOC.get(i);
        int dc = 0, dp = 0, dn = 0;
        for (int j = 0; j < APT.size(); j++) {
            Appointment& a = APT.get(j);
            if (a.getDoctorId() != d.getId() ||
                Person::compareStr(a.getDate(), today) != 0) continue;
            if (Person::compareStr(a.getStatus(), "completed") == 0) ++dc;
            else if (Person::compareStr(a.getStatus(), "pending") == 0) ++dp;
            else if (Person::compareStr(a.getStatus(), "noshow") == 0) ++dn;
        }
        if (dc + dp + dn == 0) continue;
        char cb[10], pb[10], nb[10];
        Person::intToStr(dc, cb, 10);
        Person::intToStr(dp, pb, 10);
        Person::intToStr(dn, nb, 10);
        const char* v[] = { d.getName(), cb, pb, nb };
        float fw[] = { 440, 200, 200, 200 };
        tblR(v, fw, 4, 50, 382.f + dr * 30.f, dr % 2 == 0);
        ++dr;
    }
    if (dr == 0) win.draw(mkT("No doctor activity today.", 14, P::TXTM(), 50, 384));
}
