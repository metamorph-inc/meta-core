from __future__ import absolute_import, print_function, unicode_literals

import os

import grpc
import run_mdao.progress_service.gen.progress_feedback_pb2
import run_mdao.progress_service.gen.progress_feedback_pb2_grpc

def update_progress(message, current_progress, total_progress):
    if "OPENMETA_PROGRESS_FEEDBACK_ADDR" in os.environ and "OPENMETA_PROGRESS_FEEDBACK_ID" in os.environ:
        with grpc.insecure_channel(os.environ["OPENMETA_PROGRESS_FEEDBACK_ADDR"]) as channel:
            stub = run_mdao.progress_service.gen.progress_feedback_pb2_grpc.ProgressFeedbackStub(channel)

            stub.UpdateProgress(run_mdao.progress_service.gen.progress_feedback_pb2.ProgressUpdate(
                job_id = os.environ["OPENMETA_PROGRESS_FEEDBACK_ID"],
                message = message,
                current_progress = current_progress,
                total_progress = total_progress
            ))