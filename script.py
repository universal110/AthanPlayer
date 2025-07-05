import json
from datetime import datetime

def to_24h(t):
    """Convert “hh:mm AM/PM” → “HH:MM” in 24-hour format."""
    return datetime.strptime(t, "%I:%M %p").strftime("%H:%M")

# 1) Load your JSON
with open("Yearly prayer times 2025 _ IslamicFinder.json", encoding="utf-8") as f:
    data = json.load(f)

# 2) Iterate in date order
for date in sorted(data):
    times = data[date]
    fajr    = to_24h(times["Fajr"])
    dhuhr   = to_24h(times["Dhuhr"])
    asr     = to_24h(times["Asr"])
    maghrib = to_24h(times["Maghrib"])
    isha    = to_24h(times["Isha"])
    # 3) Print your Arduino-friendly line
    print(f'testFile.println("{date}, {fajr}, {dhuhr}, {asr}, {maghrib}, {isha}");')
