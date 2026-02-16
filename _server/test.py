import time
from camera import Camera   # change this if your Camera class lives in another file


def take_photo(filename="image.jpg", preview_seconds=2):
    """
    Take a single photo using Camera class.

    :param filename: output image file name
    :param preview_seconds: how long to show preview before capture
    """
    camera = Camera()

    try:
        print("Starting camera preview...")
        camera.start_image()

        # Give camera time to auto-adjust exposure/focus
        time.sleep(preview_seconds)

        print(f"Capturing photo: {filename}")
        metadata = camera.save_image(filename)

        if metadata:
            print("Photo captured successfully.")
            return metadata
        else:
            print("Failed to capture photo.")
            return None

    finally:
        camera.close()
        print("Camera closed.")


if __name__ == "__main__":
    take_photo("test.jpg", preview_seconds=3)
