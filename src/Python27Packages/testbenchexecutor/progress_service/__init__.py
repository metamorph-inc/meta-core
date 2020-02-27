from __future__ import absolute_import, print_function, unicode_literals

import os

grpc_installed = False
try:
    import grpc
    import testbenchexecutor.progress_service.gen.progress_feedback_pb2
    import testbenchexecutor.progress_service.gen.progress_feedback_pb2_grpc
    grpc_installed = True
except ImportError:
    pass

def update_progress(message, current_progress, total_progress):
    if grpc_installed and "OPENMETA_PROGRESS_FEEDBACK_ADDR" in os.environ and "OPENMETA_PROGRESS_FEEDBACK_ID" in os.environ:
        with grpc.insecure_channel(os.environ["OPENMETA_PROGRESS_FEEDBACK_ADDR"]) as channel:
            stub = testbenchexecutor.progress_service.gen.progress_feedback_pb2_grpc.ProgressFeedbackStub(channel)

            stub.UpdateProgress(testbenchexecutor.progress_service.gen.progress_feedback_pb2.ProgressUpdate(
                job_id = os.environ["OPENMETA_PROGRESS_FEEDBACK_ID"],
                message = message,
                current_progress = current_progress,
                total_progress = total_progress
            ))