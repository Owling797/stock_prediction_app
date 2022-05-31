#!python
# -*- coding: utf-8 -*-

import socket
import codecs
import os

import torch
import torch.nn as nn
import numpy as np
import pandas as pd

from sklearn.preprocessing import MinMaxScaler
import time


def create_inout_sequences(input_data, tw):
    inout_seq = []
    L = len(input_data)
    for i in range(L - tw):
        train_seq = input_data[i : i + tw]
        train_label = input_data[i + tw : i + tw + 1]
        inout_seq.append((train_seq, train_label))
    return inout_seq


def get_predicted_data(train_data):
    scaler = MinMaxScaler(feature_range=(-1, 1))
    test_data = train_data[-5:]
    train_data = train_data[:-5]
    train_data_normalized = scaler.fit_transform(train_data.reshape(-1, 1))
    train_data_normalized = torch.FloatTensor(train_data_normalized).view(-1)
    test_data_normalized = scaler.fit_transform(test_data.reshape(-1, 1))
    test_data_normalized = torch.FloatTensor(test_data_normalized).view(-1)

    train_window = 10  # должно быть 30, но примерно похоже. если 30, то линия плавнее

    train_inout_seq = create_inout_sequences(train_data_normalized, train_window)

    class LSTM(nn.Module):
        def __init__(self, input_size=1, hidden_layer_size=100, output_size=1):
            super().__init__()
            self.hidden_layer_size = hidden_layer_size

            self.lstm = nn.LSTM(input_size, hidden_layer_size)

            self.linear = nn.Linear(hidden_layer_size, output_size)

            self.hidden_cell = (
                torch.zeros(1, 1, self.hidden_layer_size),
                torch.zeros(1, 1, self.hidden_layer_size),
            )

        def forward(self, input_seq):
            lstm_out, self.hidden_cell = self.lstm(
                input_seq.view(len(input_seq), 1, -1), self.hidden_cell
            )
            predictions = self.linear(lstm_out.view(len(input_seq), -1))
            return predictions[-1]

    model = LSTM()
    loss_function = nn.MSELoss()
    optimizer = torch.optim.Adam(model.parameters(), lr=0.001)

    print(model)

    epochs = 10

    for i in range(epochs):
        for seq, labels in train_inout_seq:
            optimizer.zero_grad()
            model.hidden_cell = (
                torch.zeros(1, 1, model.hidden_layer_size),
                torch.zeros(1, 1, model.hidden_layer_size),
            )

            y_pred = model(seq)

            single_loss = loss_function(y_pred, labels)
            single_loss.backward()
            optimizer.step()

        if i % 25 == 1:
            print(f"epoch: {i:3} loss: {single_loss.item():10.8f}")

    print(f"epoch: {i:3} loss: {single_loss.item():10.10f}")

    fut_pred = 14

    test_inputs = test_data_normalized.tolist()
    # test_inputs = train_data_normalized[-train_window:].tolist()
    # print(test_inputs)

    model.eval()

    for i in range(fut_pred):
        # seq = torch.FloatTensor(test_inputs[-train_window:])
        seq = torch.FloatTensor(test_inputs)
        with torch.no_grad():
            model.hidden = (
                torch.zeros(1, 1, model.hidden_layer_size),
                torch.zeros(1, 1, model.hidden_layer_size),
            )
            test_inputs.append(model(seq).item())

    # print(test_inputs[fut_pred:])

    actual_predictions = scaler.inverse_transform(
        np.array(test_inputs[-fut_pred:]).reshape(-1, 1)
    )
    print(actual_predictions)
    return actual_predictions


def run_server():
    ##################
    # predicted = model.predict(predict_ex)[0]
    start_time = time.time()
    WS = pd.read_excel("sber_ai758.xlsx")
    train_data = np.array(WS["Unnamed: 3"])
    scaler = MinMaxScaler(feature_range=(-1, 1))
    train_data_normalized = scaler.fit_transform(train_data.reshape(-1, 1))
    train_data_normalized = torch.FloatTensor(train_data_normalized).view(-1)

    train_window = 10  # должно быть 30, но примерно похоже. если 30, то линия плавнее

    train_inout_seq = create_inout_sequences(train_data_normalized, train_window)

    class LSTM(nn.Module):
        def __init__(self, input_size=1, hidden_layer_size=100, output_size=1):
            super().__init__()
            self.hidden_layer_size = hidden_layer_size

            self.lstm = nn.LSTM(input_size, hidden_layer_size)

            self.linear = nn.Linear(hidden_layer_size, output_size)

            self.hidden_cell = (
                torch.zeros(1, 1, self.hidden_layer_size),
                torch.zeros(1, 1, self.hidden_layer_size),
            )

        def forward(self, input_seq):
            lstm_out, self.hidden_cell = self.lstm(
                input_seq.view(len(input_seq), 1, -1), self.hidden_cell
            )
            predictions = self.linear(lstm_out.view(len(input_seq), -1))
            return predictions[-1]

    model = LSTM()
    loss_function = nn.MSELoss()
    optimizer = torch.optim.Adam(model.parameters(), lr=0.001)

    print(model)

    epochs = 10

    for i in range(epochs):
        for seq, labels in train_inout_seq:
            optimizer.zero_grad()
            model.hidden_cell = (
                torch.zeros(1, 1, model.hidden_layer_size),
                torch.zeros(1, 1, model.hidden_layer_size),
            )

            y_pred = model(seq)

            single_loss = loss_function(y_pred, labels)
            single_loss.backward()
            optimizer.step()

        if i % 25 == 1:
            print(f"epoch: {i:3} loss: {single_loss.item():10.8f}")

    print(f"epoch: {i:3} loss: {single_loss.item():10.10f}")
    # predicted = get_predicted_data(train_data)
    print("--- %s seconds ---" % (time.time() - start_time))

    ###################
    while True:
        sock = socket.socket()
        sock.bind(("127.0.0.1", 8000))  # Bind the IP address and the port number

        # 1 - максимальное количество подключений в очереди, Listen for incoming connections
        sock.listen(10)
        # accept возвращает кортеж: новый сокет и адрес клиента. Именно этот сокет и будет использоваться для приема и посылке клиенту данных.
        conn, addr = sock.accept()

        # print("connected:", addr)

        # while True:
        conn.settimeout(60)
        print("READY")
        data = conn.recv(10240)  # забрать 1024 байта
        # if data:
        print(data, type(data))
        # data = str(data)

        input_values = codecs.decode(data, "utf-8")
        print(input_values, type(input_values))

        input_values = input_values.split()
        input_values = [int(i) for i in input_values]
        print(input_values, type(input_values), type(input_values[0]))
        # num1, num2, num3, num4, num5 = [i for i in input_values]

        ### вызов
        predict_ex = np.array(
            [
                input_values[0],
                input_values[1],
                input_values[2],
                input_values[3],
                input_values[4],
            ]
        )
        ###################

        test_data_normalized = scaler.fit_transform(predict_ex.reshape(-1, 1))
        test_data_normalized = torch.FloatTensor(test_data_normalized).view(-1)
        fut_pred = 7
        test_inputs = test_data_normalized.tolist()
        model.eval()

        for i in range(fut_pred):
            seq = torch.FloatTensor(test_inputs)
            with torch.no_grad():
                model.hidden = (
                    torch.zeros(1, 1, model.hidden_layer_size),
                    torch.zeros(1, 1, model.hidden_layer_size),
                )
                test_inputs.append(model(seq).item())

        actual_predictions = scaler.inverse_transform(
            np.array(test_inputs[-fut_pred:]).reshape(-1, 1)
        )
        print(actual_predictions)
        #####################
        # print(predicted)

        result = np.asarray(actual_predictions).ravel().tolist()

        output_values = ""
        for i in result:
            output_values += str(i) + " "
        print(result)
        output_values = output_values[:-1]
        # print(output_values)

        # conn.send(data.upper())  # вернуть
        outp_data = output_values.encode()
        # sock.connect(addr | output_values.encode())
        conn.send(outp_data)
        # for i in len(input_values):
        #   input_values = int(i)

        conn.close()


if __name__ == "__main__":
    # while True:
    run_server()
