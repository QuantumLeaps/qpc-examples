//============================================================================
// Product: DPP example, GTK3+-GUI
// Last updated for version 8.0.0
// Last updated on  2024-09-18
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Alternatively, this program may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which expressly supersede
// the GNU General Public License and are specifically designed for
// licensees interested in retaining the proprietary status of their code.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <www.gnu.org/licenses/>.
//
// Contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#include <unistd.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

#include "dppImages.h"

#include "safe_std.h" // portable "safe" <stdio.h>/<string.h> facilities

Q_DEFINE_THIS_FILE

// local types ---------------------------------------------------------------
typedef enum picIdx {
    UNDEF = -1,
    THINK = 0,
    HUNGRY,
    EAT
} picIdx_t;

// local variables -----------------------------------------------------------
#ifdef Q_SPY
    enum {
        PHILO_STAT = QS_USER
    };
    static uint8_t const l_clock_tick = 0U;
#endif

static GMutex mymutex;
static GThread *tlThread = NULL;
static GRand *myRnd = NULL;

static GtkApplication* l_app = NULL;
static GtkWidget *l_hwnd = NULL;
static GtkWidget *philos[N_PHILO] = {NULL};
static GtkWidget *texts[2] = {NULL, NULL};

static const gchar *iconPlay =  "media-playback-start";
static const gchar *iconPause = "media-playback-pause";
static const gchar *philoPics[3] = {
    "/dpp/example/pics/think",
    "/dpp/example/pics/hungry",
    "/dpp/example/pics/eat"
};
static const guint8 philoPos[N_PHILO][2] = {
    {2, 0}, {4, 1}, {3, 2}, {1, 2}, {0, 1}
};

static gboolean requestQuit = FALSE;

static gchar *l_cmdLine = NULL;
static gchar *l_message = NULL;
static gchar *l_caption = NULL;
static picIdx_t philoState[N_PHILO];

// Local functions -----------------------------------------------------------
static gint theApp(GtkApplication *app);
static gint MessageBox(const gchar *text, const gchar *caption, gboolean yesNo);
static void ButtonClicked(GtkButton *btn, GtkWidget *widget);
static void copy(GtkButton *btn, GtkWidget *widget);
static void on_close_request(GtkWidget *window);
static gboolean eventHandler(GtkWidget *widget);
static void notify(gchar *message, gchar *caption, gboolean wantToQuit);

//............................................................................
// simulte Win32 function MessageBox(...)
static gint MessageBox(const gchar *text, const gchar *caption, gboolean yesNo)
{
    gint result = GTK_RESPONSE_NO;

    GtkWidget *dialog  = gtk_dialog_new();
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *message = gtk_label_new(text);
    GtkWidget *icon    = gtk_image_new_from_icon_name(yesNo ? "dialog-question-symbolic" : "dialog-information-symbolic", GTK_ICON_SIZE_DIALOG);

    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(NULL));
    gtk_window_set_modal(GTK_WINDOW(dialog), FALSE);
    gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
    gtk_window_set_keep_above(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(dialog), caption);
    gtk_container_add(GTK_CONTAINER(content), icon);
    gtk_container_add(GTK_CONTAINER(content), message);
    gtk_dialog_add_button(GTK_DIALOG(dialog),  yesNo ? "_Yes" : "_OK", GTK_RESPONSE_OK);
    if(yesNo)
    {
        gtk_dialog_add_button(GTK_DIALOG(dialog),  "_No", GTK_RESPONSE_NO);
    }
    gtk_widget_set_size_request(dialog, 250, -1);
    gtk_widget_show_all(dialog);

    result = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    switch(result)
    {
        case GTK_RESPONSE_YES:
        case GTK_RESPONSE_OK:
            return GTK_RESPONSE_OK;
        default:
            return GTK_RESPONSE_NO;
    }

    return GTK_RESPONSE_OK;
}

//............................................................................
Q_NORETURN Q_onError(char const * const module, int_t const id) {
    char message[80];
    QF_stop(); // stop ticking

    QS_ASSERTION(module, id, 10000U); // report assertion to QS
    SNPRINTF_S(message, Q_DIM(message) - 1U,
               "Assertion failed in module %s location %d", module, id);
    //fprintf(stderr, "%s\n", message);
    g_mutex_lock(&mymutex);
    notify(message, "!!! ASSERTION !!!", TRUE);
    g_mutex_unlock(&mymutex);
    QF_onCleanup();
    QS_EXIT();
}
//............................................................................
void assert_failed(char const * const module, int_t const id); // prototype
void assert_failed(char const * const module, int_t const id)
{
    Q_onError(module, id);
}

//----------------------------------------------------------------------------
#ifdef main
#undef main
#endif
int main(int argc, char *argv[])
{
    gint status;

    if(argc > 1)
    {
        l_cmdLine = g_strdup(argv[1]);
        argc = 1;
        argv[1] = NULL;
    }

    g_mutex_init(&mymutex);
    g_mutex_lock(&mymutex);

    myRnd = g_rand_new();
    tlThread = g_thread_new(NULL, (GThreadFunc)main_gui, NULL);
    l_app = gtk_application_new ("com.state-machine.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect_swapped(l_app, "activate", G_CALLBACK(theApp), l_app);

    status = g_application_run (G_APPLICATION(l_app), argc, argv);

    BSP_terminate(0);

    return status;
}

//............................................................................
static gint theApp(GtkApplication *app)
{
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    l_hwnd = window;
    gtk_application_add_window(GTK_APPLICATION(app), GTK_WINDOW(window));

    GdkPixbuf *icon = gdk_pixbuf_new_from_resource("/dpp/example/icons/qp", NULL);

    GtkWidget *smLabel = gtk_label_new(NULL);
    GtkWidget *empty   = gtk_label_new("");

    GtkWidget *button = gtk_button_new();
    GtkWidget *btnCopy = gtk_button_new_with_label("Copy");
    GtkWidget *btnQuit = gtk_button_new_from_icon_name("application-exit", GTK_ICON_SIZE_BUTTON);

    GtkWidget *editBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
    GtkWidget *btnQuitBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    GtkWidget *btnPlayBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);

    GString *winTitle = g_string_new("DPP Example");

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 4);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 4);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

    gtk_label_set_markup(GTK_LABEL(smLabel), "<span foreground=\"blue\" size=\"smaller\"><i>https://www.state-machine.com</i></span>");

    // init the philo state array
    for(gint n = 0; n < N_PHILO; n++)
    {
        philoState[n] = UNDEF;
    }

    for(gint i = 0; i < 2; i++)
    {
        gchar text[16];
        texts[i] = gtk_entry_new();
        sprintf(text, "Edit %1d ...", i + 1);
        gtk_entry_set_text(GTK_ENTRY(texts[i]), text);
    }

    g_string_append_printf (winTitle, " GTK %u.%u", gtk_get_major_version(), gtk_get_minor_version());
    gtk_window_set_title (GTK_WINDOW (window), (const gchar *)winTitle->str);
    gtk_window_set_resizable(GTK_WINDOW (window), FALSE);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size (GTK_WINDOW (window), 420, 250);
    gtk_window_set_icon(GTK_WINDOW (window), icon);

    gtk_button_set_always_show_image(GTK_BUTTON(btnQuit), TRUE);
    gtk_button_set_image(GTK_BUTTON(button), gtk_image_new_from_icon_name(iconPause, GTK_ICON_SIZE_BUTTON));
    gtk_button_set_always_show_image(GTK_BUTTON(button), TRUE);

    gtk_box_pack_start(GTK_BOX(editBox), texts[0], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(editBox), btnCopy, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(editBox), texts[1], TRUE, TRUE, 0);

    for(gint i = 0; i < N_PHILO; i++)
    {
        philos[i] = gtk_image_new_from_resource(philoPics[HUNGRY]);
        gtk_grid_attach(GTK_GRID(grid), philos[i], philoPos[i][0], philoPos[i][1], 1, 1);
    }

    gtk_button_box_set_layout(GTK_BUTTON_BOX(btnQuitBox), GTK_BUTTONBOX_END);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(btnPlayBox), GTK_BUTTONBOX_CENTER);

    gtk_box_pack_start(GTK_BOX(btnPlayBox), button, TRUE, FALSE, 10);
    gtk_box_pack_end(GTK_BOX(btnQuitBox), btnQuit, TRUE, FALSE, 10);

    gtk_grid_attach(GTK_GRID(grid), btnPlayBox, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), empty, 0, 3, 5, 1);
    gtk_grid_attach(GTK_GRID(grid), editBox, 0, 4, 5, 1);
    gtk_grid_attach(GTK_GRID(grid), btnQuitBox, 4, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), smLabel, 0, 6, 5, 1);

    g_signal_connect_swapped(G_OBJECT(button), "clicked", G_CALLBACK(ButtonClicked), button);
    g_signal_connect_swapped(G_OBJECT(btnQuit), "clicked", G_CALLBACK(gtk_widget_destroy), window);
    g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(on_close_request), window);
    g_signal_connect_swapped(G_OBJECT(btnCopy), "clicked", G_CALLBACK(copy), btnCopy);

    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(grid));
    gdk_threads_add_idle((GSourceFunc)eventHandler, window);
    gtk_widget_show_all (window);

    return 0;
}

//............................................................................
static gboolean eventHandler(GtkWidget *window)
{
    static  gboolean firstRun = true;
    gboolean retCode = TRUE;

    if(firstRun)
    {
        firstRun = false;
        g_mutex_unlock(&mymutex);
    }

    if(g_mutex_trylock(&mymutex))
    {
        if(l_message)
        {
            MessageBox(l_message, l_caption ? l_caption : "Message!", FALSE);
            free(l_message);
            l_message = NULL;
            free(l_caption);
            l_caption = NULL;
        }
        else for(gint n = 0; n < N_PHILO; n++)
        {
            gint l = philoState[n];
            if(l >= 0)
            {
                gtk_image_set_from_resource(GTK_IMAGE(philos[n]), philoPics[l]);
                philoState[n] = -1;
            }
        }
        if(requestQuit) {
            requestQuit = FALSE;
            if(l_hwnd)
                gtk_widget_destroy(l_hwnd);
            retCode = FALSE;
        }

        g_mutex_unlock(&mymutex);
    }
    return retCode;
}

//............................................................................
static void ButtonClicked(GtkButton *btn, GtkWidget *widget)
{
    static guint8 peIdx = 0x00u;
    static GtkWidget *btnImg;
    static const QEvt pe[2] = {
        QEVT_INITIALIZER(PAUSE_SIG),
        QEVT_INITIALIZER(SERVE_SIG)
    };

    btnImg = gtk_image_new_from_icon_name((peIdx ? iconPause : iconPlay), GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(btn), btnImg);

    QACTIVE_POST(AO_Table, &pe[peIdx], (void *)0);
    peIdx++;
    peIdx &= 0x01u;
}

//............................................................................
static void copy(GtkButton *btn, GtkWidget *widget)
{
    const gchar *s = gtk_entry_get_text(GTK_ENTRY(texts[0]));

    gtk_entry_set_text(GTK_ENTRY(texts[1]), s);
}

//............................................................................
static void on_close_request(GtkWidget *window)
{
    notify("\n\n           Bye bye!\nSee you again next time...\n\n", "Good bye", TRUE);
    gtk_widget_destroyed(window, &l_hwnd);
}

//............................................................................
static void notify(gchar *message, gchar *caption, gboolean wantToQuit)
{
    l_message = strdup(message);
    l_caption = strdup(caption);
    requestQuit = wantToQuit;
}
//............................................................................
void QF_onStartup(void) {
    QF_setTickRate(BSP_TICKS_PER_SEC, 30); // set the desired tick rate
}
//............................................................................
void QF_onCleanup(void) {
    g_thread_exit((gpointer)0);
}
//............................................................................
void QF_onClockTick(void) {
    QTIMEEVT_TICK_X(0U, &l_clock_tick); // QF clock tick processing

    QS_RX_INPUT(); // handle the QS-RX input
    QS_OUTPUT();   // handle the QS output
}

//............................................................................
void BSP_init(void) {
    g_mutex_lock(&mymutex);
    if (!QS_INIT(l_cmdLine)) { // QS initialization failed?
        notify("Cannot connect to QSPY via TCP/IP\n"
                "Please make sure that 'qspy -t' is running",
                "QS_INIT() Error", TRUE);
    }
    g_mutex_unlock(&mymutex);

    // send the QS dictionaries...
    QS_OBJ_DICTIONARY(&l_clock_tick);
    QS_USR_DICTIONARY(PHILO_STAT);

    // setup the QS filters...
    QS_GLB_FILTER(QS_ALL_RECORDS);
    QS_GLB_FILTER(-QS_QF_TICK);
}
//............................................................................
void BSP_start(void) {
    // initialize event pools
    static QF_MPOOL_EL(TableEvt) smlPoolSto[2*N_PHILO];
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    // initialize publish-subscribe
    static QSubscrList subscrSto[MAX_PUB_SIG];
    QActive_psInit(subscrSto, Q_DIM(subscrSto));

    // instantiate and start AOs/threads...

    static QEvtPtr philoQueueSto[N_PHILO][10];
    for (uint8_t n = 0U; n < N_PHILO; ++n) {
        Philo_ctor(n);
        QActive_start(AO_Philo[n],
            n + 3U,                  // QF-prio/pthre. see NOTE1
            philoQueueSto[n],        // event queue storage
            Q_DIM(philoQueueSto[n]), // queue length [events]
            (void *)0, 0U,           // no stack storage
            (void *)0);              // no initialization param
    }

    static QEvtPtr tableQueueSto[N_PHILO];
    Table_ctor();
    QActive_start(AO_Table,
        N_PHILO + 7U,                // QP prio. of the AO
        tableQueueSto,               // event queue storage
        Q_DIM(tableQueueSto),        // queue length [events]
        (void *)0, 0U,               // no stack storage
        (void *)0);                  // no initialization param
}
//............................................................................
void BSP_terminate(gint16 result) {
    QF_stop(); // stop the main QF application

    // cleanup all QWIN resources...

    // end the main dialog

}

//............................................................................
void BSP_displayPhilStat(uint8_t n, char const *stat)
{
    picIdx_t bitmapNum = THINK;

    Q_REQUIRE(n < N_PHILO);

    switch (stat[0]) {
        case 't': bitmapNum = THINK; break;
        case 'h': bitmapNum = HUNGRY; break;
        case 'e': bitmapNum = EAT; break;
        default: Q_ERROR();  break;
    }
    // set the "segment" # n to the bitmap # 'bitmapNum'
    // gtk_image_set_from_resource(GTK_IMAGE(philos[n]), philoPics[bitmapNum]);
    g_mutex_lock(&mymutex);
    philoState[n] = bitmapNum;
    g_mutex_unlock(&mymutex);

    QS_BEGIN_ID(PHILO_STAT, AO_Philo[n]->prio) // app-specific record
        QS_U8(1, n);  // Philosopher number
        QS_STR(stat); // Philosopher status
    QS_END()
}
//............................................................................
void BSP_displayPaused(guint8 paused)
{
    // already done in button callback
}
//............................................................................
guint32 BSP_random(void) {  // a very cheap pseudo-random-number generator
    // "Super-Duper" Linear Congruential Generator (LCG)
    // LCG(2^32, 3*7*11*13*23, 0, seed)
    //
    return g_rand_int(myRnd) >> 8;
}
//............................................................................
void BSP_randomSeed(guint32 seed) {
    g_rand_set_seed (myRnd, seed);
}


//----------------------------------------------------------------------------
#ifdef Q_SPY // define QS callbacks

//! callback function to execute user commands
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    switch (cmdId) {
       case 0U: {
           break;
       }
       default:
           break;
    }

    // unused parameters
    (void)param1;
    (void)param2;
    (void)param3;
}

#endif // Q_SPY
//----------------------------------------------------------------------------
