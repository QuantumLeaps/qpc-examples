# This is an example of QView customization for a specific application
# (DPP in this case). This example animates the Phil images on the
# QView canvas. Additionally, there is a button in the middle of the screen,
# which, when clicked once pauses the DPP ("forks" are not being served).
# A second click on the button, "un-pauses" the DPP ("forks" are served
# to all hungry Philosophers).
#
# This version of the DPP customization uses the application-specific
# trace record QS_USER_00 (PHILO_STAT) produced when the status of
# a Philo changes.

from qview import QView
from tkinter import *
from tkinter.ttk import * # override the basic Tk widgets with Ttk widgets

class DPP(QView):
    # on_init() callback
    def on_init(self):
        # add commands to the Custom menu...
        QView.custom_menu.add_command(label="Custom command",
                                      command=self.cust_command)

        # configure the custom QView.canvas...
        QView.show_canvas() # make the canvas visible
        QView.canvas.configure(width=400, height=260)

        # tuple of activity images (correspond to self._philo_state)
        self._act_img = (
            PhotoImage(file="img/thinking.gif"),
            PhotoImage(file="img/hungry.gif"),
            PhotoImage(file="img/eating.gif"),
        )
        # tuple of philo canvas images (correspond to self._philo_obj)
        self._philo_img = (\
            QView.canvas.create_image(190,  57, image=self._act_img[0]),
            QView.canvas.create_image(273, 100, image=self._act_img[0]),
            QView.canvas.create_image(237, 185, image=self._act_img[0]),
            QView.canvas.create_image(146, 185, image=self._act_img[0]),
            QView.canvas.create_image(107, 100, image=self._act_img[0])
        )

        # button images for UP and DOWN
        self.img_UP  = PhotoImage(file="img/BTN_UP.gif")
        self.img_DWN = PhotoImage(file="img/BTN_DWN.gif")

        # images of a button for pause/serve
        self.btn = QView.canvas.create_image(200, 120, image=self.img_UP)
        QView.canvas.tag_bind(self.btn, "<ButtonPress-1>", self.cust_pause)

        # request target reset on startup...
        # NOTE: Normally, for an embedded application you would like
        # to start with resetting the Target, to start clean with
        # QS dictionaries, etc.
        QView.reset_target()


    # on_reset() callback invoked when Target-reset packet is received
    # NOTE: the QS dictionaries are not known at this time yet, so
    # this callback shouild generally not set filters or current objects
    def on_reset(self):
        # clear the lists
        self._philo_obj   = [0, 0, 0, 0, 0]
        self._philo_state = [0, 0, 0]

    # on_run() callback invoked when the QF_RUN packet is received
    # NOTE: the QS dictionaries are typically known at this time yet, so
    # this callback can set filters or current objects
    def on_run(self):
        QView.glb_filter("QS_USER_00")

        # NOTE: the names of objects for current_obj() must match
        # the QS Object Dictionaries produced by the application.
        QView.current_obj(QView.OBJ_AO, "Table_inst")

        # turn lists into tuples for better performance
        self._philo_obj = tuple(self._philo_obj)
        self._philo_state = tuple(self._philo_state)


    # example of a custom command
    def cust_command(self):
        QView.command(1, 12345)

    # example of a custom interaction with a canvas object (pause/serve)
    def cust_pause(self, event):
        if QView.canvas.itemcget(self.btn, "image") != str(self.img_UP):
            QView.canvas.itemconfig(self.btn, image=self.img_UP)
            QView.post("SERVE_SIG")
            QView.print_text("Table SERVING")
        else:
            QView.canvas.itemconfig(self.btn, image=self.img_DWN)
            QView.post("PAUSE_SIG")
            QView.print_text("Table PAUSED")

    # Intercept the QS_USER_00 application-specific trace record.
    # This record has the following structure (see bsp.c:displayPhilStat()):
    # Seq-Num, Record-ID, Timestamp, format-byte, Philo-num,
    #    format-byte, Zero-terminated string (status)
    def QS_USER_00(self, packet):
        # unpack: Timestamp->data[0], Philo-num->data[1], status->data[3]
        data = QView.qunpack("xxTxBxZ", packet)
        i = data[1]
        j = ("t", "h", "e").index(data[2][0]) # the first letter

        # animate the given philo image according to its activity
        QView.canvas.itemconfig(self._philo_img[i], image=self._act_img[j])

        # print a message to the text view
        QView.print_text("%010d Philo %1d is %s"%(data[0], i, data[2]))

#=============================================================================
if __name__ == "__main__":
    QView.main(DPP())
