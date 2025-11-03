import json
from datetime import datetime

def to_24h(t):
    """Convert “hh:mm AM/PM” → “HH:MM” in 24-hour format."""
    return datetime.strptime(t, "%I:%M %p").strftime("%H:%M")

with open("Yearly prayer times 2025 _ IslamicFinder.json", encoding="utf-8") as f:
    data = json.load(f)

for date in sorted(data):
    times = data[date]
    fajr    = to_24h(times["Fajr"])
    dhuhr   = to_24h(times["Dhuhr"])
    asr     = to_24h(times["Asr"])
    maghrib = to_24h(times["Maghrib"])
    isha    = to_24h(times["Isha"])

    print(f'testFile.println("{date}, {fajr}, {dhuhr}, {asr}, {maghrib}, {isha}");')
