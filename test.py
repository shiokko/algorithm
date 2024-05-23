import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from torch.utils.data import DataLoader

import nltk
from nltk.corpus import movie_reviews
from nltk.tokenize import word_tokenize
nltk.download("movie_reviews")

from collections import defaultdict, Counter
import math
import random

random.seed(0) # Don't change
torch.manual_seed(0)  # Don't change
np.random.seed(0) # Don't change

train_X, train_Y = [], []
test_X, test_Y = [], []

for polarity in movie_reviews.categories():
    label = 0 if polarity == 'neg' else 1
    for fid in movie_reviews.fileids(polarity):
        if random.randrange(5) == 0:
            test_X.append(movie_reviews.raw(fid))
            test_Y.append(label)
        else:
            train_X.append(movie_reviews.raw(fid))
            train_Y.append(label)
print(train_X[0], train_Y[0])

from simpletransformers.classification import ClassificationModel
import pandas as pd
import logging

logging.basicConfig(level=logging.INFO)
transformers_logger = logging.getLogger("transformers")
transformers_logger.setLevel(logging.WARNING)

def build_model(training_instances, training_labels):
    # 如果 GPU 可用，可以将 use_cuda 设置为 True
    model = ClassificationModel("bert", "bert-base-uncased", use_cuda=True, args={
        'overwrite_output_dir': True,
        'num_train_epochs': 3,
        'learning_rate': 1e-5*8,  # 调整学习率
        'train_batch_size': 16,  # 调整批次大小
        'eval_batch_size': 16,
        'evaluate_during_training': True,  # 在训练过程中进行评估
        'evaluate_during_training_steps': 1000,
        'evaluate_during_training_verbose': True,
    })

    # 将训练实例和标签组合成 DataFrame
    train_data = {"text": training_instances, "labels": training_labels}
    train_df = pd.DataFrame(train_data)

    # 使用 DataFrame 进行模型训练，并提供验证集进行评估
    model.train_model(train_df, eval_df=train_df)

    return model

model = build_model(train_X, train_Y)

def predict(model, document):
    return model.predict([document])[0][0]

print(predict(model, "this is an uninteresting movie"))
print(predict(model, "a good movie of this year"))


correct, total = 0, 0

pred_Y = model.predict(test_X)[0]

for prediction, y in zip(pred_Y, test_Y):
    if prediction == y:
        correct += 1
    total += 1

print("%d / %d = %g" % (correct, total, correct / total))