import pytest
from unittest.mock import MagicMock, patch
import threading

# Import your module
import camera


# ----------------------------
# StreamingOutput Tests
# ----------------------------

def test_streaming_output_write_sets_frame():
    output = camera.StreamingOutput()

    data = b"test_frame"
    size = output.write(data)

    assert output.frame == data
    assert size == len(data)


# ----------------------------
# Camera Initialization
# ----------------------------

@patch("camera.Picamera2")
@patch("camera.Transform")
def test_camera_init(mock_transform, mock_picamera):
    mock_cam = MagicMock()
    mock_picamera.return_value = mock_cam

    cam = camera.Camera()

    mock_picamera.assert_called_once()
    mock_cam.create_preview_configuration.assert_called_once()
    mock_cam.configure.assert_called_once()

    assert cam.streaming is False


# ----------------------------
# save_image
# ----------------------------

@patch("camera.Picamera2")
@patch("camera.Transform")
def test_save_image_success(mock_transform, mock_picamera):
    mock_cam = MagicMock()
    mock_cam.capture_file.return_value = {"meta": "data"}
    mock_picamera.return_value = mock_cam

    cam = camera.Camera()

    result = cam.save_image("test.jpg")

    assert result == {"meta": "data"}
    mock_cam.capture_file.assert_called_once_with("test.jpg")


@patch("camera.Picamera2")
@patch("camera.Transform")
def test_save_image_failure(mock_transform, mock_picamera):
    mock_cam = MagicMock()
    mock_cam.capture_file.side_effect = Exception("boom")
    mock_picamera.return_value = mock_cam

    cam = camera.Camera()

    result = cam.save_image("test.jpg")

    assert result is None


# ----------------------------
# start_stream / stop_stream
# ----------------------------

@patch("camera.FileOutput")
@patch("camera.H264Encoder")
@patch("camera.Picamera2")
@patch("camera.Transform")
def test_start_and_stop_stream(mock_transform, mock_picamera, mock_encoder, mock_output):
    mock_cam = MagicMock()
    mock_cam.started = False
    mock_picamera.return_value = mock_cam

    cam = camera.Camera()

    cam.start_stream("video.h264")

    mock_cam.configure.assert_called()
    mock_cam.start_recording.assert_called()
    assert cam.streaming is True

    cam.stop_stream()

    mock_cam.stop_recording.assert_called_once()
    assert cam.streaming is False


# ----------------------------
# get_frame
# ----------------------------

def test_get_frame_returns_written_frame():
    cam = camera.Camera.__new__(camera.Camera)
    cam.streaming_output = camera.StreamingOutput()

    frame = b"hello"

    def writer():
        cam.streaming_output.write(frame)

    threading.Thread(target=writer).start()

    result = cam.get_frame()

    assert result == frame


# ----------------------------
# close
# ----------------------------

@patch("camera.Picamera2")
@patch("camera.Transform")
def test_close_calls_camera_close(mock_transform, mock_picamera):
    mock_cam = MagicMock()
    mock_picamera.return_value = mock_cam

    cam = camera.Camera()
    cam.close()

    mock_cam.close.assert_called_once()
