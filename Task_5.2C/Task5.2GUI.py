import tkinter as tk
from gpiozero import LED, PWMLED


# ---------------- GPIO SETUP ----------------

# Living Room uses PWM for brightness control
living_led = PWMLED(14)

# Other lights remain normal LEDs
bathroom_led = LED(15)
closet_led = LED(18)


# ---------------- LED FUNCTIONS ----------------

def change_light(led, button):
    if led.is_lit:
        led.off()
        button.config(text="OFF", bg="red")
    else:
        led.on()
        button.config(text="ON", bg="green")


def change_living_brightness(value):
    brightness = float(value) / 100
    living_led.value = brightness

    # If brightness is greater than 0, show ON
    if brightness > 0:
        living_button.config(text="ON", bg="green")
    else:
        living_button.config(text="OFF", bg="red")


def turn_everything_on():
    living_led.on()
    bathroom_led.on()
    closet_led.on()

    living_button.config(text="ON", bg="green")
    bathroom_button.config(text="ON", bg="green")
    closet_button.config(text="ON", bg="green")


def turn_everything_off():
    living_led.off()
    bathroom_led.off()
    closet_led.off()

    living_button.config(text="OFF", bg="red")
    bathroom_button.config(text="OFF", bg="red")
    closet_button.config(text="OFF", bg="red")


def exit_application():
    turn_everything_off()
    app.destroy()


# ---------------- MAIN WINDOW ----------------

app = tk.Tk()

app.title("Raspberry Pi Light Controller")
app.geometry("550x680")
app.configure(bg="#1f1f1f")
app.resizable(False, False)


# ---------------- TITLE ----------------

title = tk.Label(
    app,
    text="HOME LIGHT CONTROL",
    font=("Arial", 24, "bold"),
    fg="white",
    bg="#1f1f1f"
)

title.pack(pady=(30, 10))


subtitle = tk.Label(
    app,
    text="Control your room lights",
    font=("Arial", 14),
    fg="#cccccc",
    bg="#1f1f1f"
)

subtitle.pack(pady=(0, 25))


# ---------------- ROOM FRAME ----------------

room_frame = tk.Frame(
    app,
    bg="#2b2b2b",
    padx=25,
    pady=20
)

room_frame.pack(padx=30, fill="x")


# ---------------- LIVING ROOM ----------------

living_label = tk.Label(
    room_frame,
    text="Living Room",
    font=("Arial", 16, "bold"),
    fg="white",
    bg="#2b2b2b"
)

living_label.grid(row=0, column=0, padx=10, pady=12)


living_button = tk.Button(
    room_frame,
    text="OFF",
    width=10,
    font=("Arial", 13, "bold"),
    bg="red",
    fg="white",
    command=lambda: change_light(living_led, living_button)
)

living_button.grid(row=0, column=1, padx=10, pady=12)


# ---------------- BRIGHTNESS SLIDER ----------------

brightness_label = tk.Label(
    room_frame,
    text="Brightness",
    font=("Arial", 12),
    fg="white",
    bg="#2b2b2b"
)

brightness_label.grid(row=1, column=0, padx=10, pady=5)


brightness_slider = tk.Scale(
    room_frame,
    from_=0,
    to=100,
    orient=tk.HORIZONTAL,
    length=180,
    showvalue=True,
    bg="#2b2b2b",
    fg="white",
    highlightthickness=0,
    command=change_living_brightness
)

brightness_slider.set(100)

brightness_slider.grid(row=1, column=1, padx=10, pady=5)


# ---------------- BATHROOM ----------------

bathroom_label = tk.Label(
    room_frame,
    text="Bathroom",
    font=("Arial", 16, "bold"),
    fg="white",
    bg="#2b2b2b"
)

bathroom_label.grid(row=2, column=0, padx=10, pady=12)


bathroom_button = tk.Button(
    room_frame,
    text="OFF",
    width=10,
    font=("Arial", 13, "bold"),
    bg="red",
    fg="white",
    command=lambda: change_light(bathroom_led, bathroom_button)
)

bathroom_button.grid(row=2, column=1, padx=10, pady=12)


# ---------------- CLOSET ----------------

closet_label = tk.Label(
    room_frame,
    text="Closet",
    font=("Arial", 16, "bold"),
    fg="white",
    bg="#2b2b2b"
)

closet_label.grid(row=3, column=0, padx=10, pady=12)


closet_button = tk.Button(
    room_frame,
    text="OFF",
    width=10,
    font=("Arial", 13, "bold"),
    bg="red",
    fg="white",
    command=lambda: change_light(closet_led, closet_button)
)

closet_button.grid(row=3, column=1, padx=10, pady=12)


# ---------------- MASTER CONTROLS ----------------

control_frame = tk.Frame(
    app,
    bg="#1f1f1f"
)

control_frame.pack(pady=30)


all_on_button = tk.Button(
    control_frame,
    text="ALL ON",
    width=12,
    font=("Arial", 13, "bold"),
    bg="blue",
    fg="white",
    command=turn_everything_on
)

all_on_button.grid(row=0, column=0, padx=8)


all_off_button = tk.Button(
    control_frame,
    text="ALL OFF",
    width=12,
    font=("Arial", 13, "bold"),
    bg="gray",
    fg="white",
    command=turn_everything_off
)

all_off_button.grid(row=0, column=1, padx=8)


# ---------------- EXIT ----------------

exit_button = tk.Button(
    app,
    text="EXIT",
    width=15,
    font=("Arial", 13, "bold"),
    bg="#8b0000",
    fg="white",
    command=exit_application
)

exit_button.pack(pady=10)


# Turn LEDs off if window is closed using X
app.protocol("WM_DELETE_WINDOW", exit_application)


# ---------------- START GUI ----------------

app.mainloop()