import serial
import serial.tools.list_ports

BAUDRATE = 115200
SCREEN_WIDTH  = 240
SCREEN_HEIGHT = 320
ADC_MIN = 300
ADC_MAX = 3700


def find_port() -> str | None:
    for port in serial.tools.list_ports.comports():
        if "usbmodem" in port.description or "usbmodem" in port.device:
            print(f"Found device on {port.device}")
            return port.device
    return None


def parse_line(line: str) -> tuple[float, float] | None:
    try:
        parts = line.strip().split(',')
        if len(parts) != 2:
            return None
        return float(parts[0]), float(parts[1])
    except ValueError:
        return None


def is_valid(x: float, y: float) -> bool:
    return ADC_MIN < x < ADC_MAX and ADC_MIN < y < ADC_MAX


def to_screen(adc_x: float, adc_y: float, cal: dict) -> tuple[int, int]:
    x_range = cal['x_max'] - cal['x_min']
    y_range = cal['y_max'] - cal['y_min']
    if x_range == 0 or y_range == 0:
        return 0, 0
    sx = (adc_x - cal['x_min']) / x_range * SCREEN_WIDTH
    sy = (adc_y - cal['y_min']) / y_range * SCREEN_HEIGHT
    return int(sx), int(sy)


def main():
    target_port = find_port()
    if target_port is None:
        print("Error: device not found")
        return

    try:
        ser = serial.Serial(target_port, BAUDRATE, timeout=5)
    except serial.SerialException as e:
        print(f"Error opening port: {e}")
        return

    cal = None

    print("Swipe across all corners. Press Ctrl+C to finish.\n")

    try:
        while True:
            try:
                raw = ser.readline().decode('utf-8')
            except UnicodeDecodeError:
                continue

            point = parse_line(raw)
            if point is None:
                continue

            x, y = point

            if not is_valid(x, y):
                continue

            if cal is None:
                cal = {'x_min': x, 'x_max': x, 'y_min': y, 'y_max': y}
            else:
                cal['x_min'] = min(cal['x_min'], x)
                cal['x_max'] = max(cal['x_max'], x)
                cal['y_min'] = min(cal['y_min'], y)
                cal['y_max'] = max(cal['y_max'], y)

            sx, sy = to_screen(x, y, cal)

            print(f"ADC ({x:.0f}, {y:.0f}) → Screen ({sx}, {sy})  "
                  f"| Cal x=[{cal['x_min']:.0f}, {cal['x_max']:.0f}]"
                  f"  y=[{cal['y_min']:.0f}, {cal['y_max']:.0f}]")

    except KeyboardInterrupt:
        if cal:
            print(f"\nFinal calibration:")
            print(f"  x_min={cal['x_min']:.0f}, x_max={cal['x_max']:.0f}")
            print(f"  y_min={cal['y_min']:.0f}, y_max={cal['y_max']:.0f}")
        else:
            print("\nNo data collected.")
    finally:
        ser.close()


if __name__ == "__main__":
    main()