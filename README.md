# Projeto 2

Projeto 2 da disciplina de Fundamentos de Sistemas Embarcados (2020/2)

## 1. Objetivos

Este trabalho tem por objetivo a criação de um sistema distribuído de automação residencial para monitoramento e acionamento de sensores e dispositivos. O sistema deve ser desenvolvido para funcionar em um conjunto de placas Raspberry Pi com um servidor central responsável pelo controle e interface com o usuário e servidores distribuídos para leitura e acionamento dos dispositivos. Dentre os dispositivos envolvidos estão o monitoramento de temperatura, sensores de presença, sensores de abertura e fechamento de portas e janelas, acionamento de lâmpadas, aparelhos de ar-condicionado e alarme.

![Figura](/figuras/arquitetura.png)

## 2. Componentes do Sistema

Para simplificar o trabalho focamos o escopo em somente um servidor central e um servidor distribuído.

O sistema do Servidor Central será composto por:
1. 01 Placa Raspberry Pi 4;
2. Saída de som (caixa de som para alarme);

O sistema do Servidor Distribuído será composto por:
1. 01 Placa Raspberry Pi 4;
2. 01 Sensor BME280 (I2C) para a medição da temperatura / umidade;
3. Circuito de potência com 6 relés para acionametno de Lâmpadas / Aparelhos de Ar-Condicionado;
4. 06 Sensores de fechamento de portas/janelas;
5. 02 sensores de presença;

## 3. Conexões entre os módulos do sistema

1. Os servidores deverão se comunicar através do Protocolo TCP/IP;
2. O alarme do servidor Central deverá ser acionado tocando um arquivo de áudio pela saída de som da própria placa (Uma opção é o uso do omxplayer - https://www.raspberrypi.org/documentation/usage/audio/);
3. O sensor de temperatura BME280 está ligado ao barramento I2C do servidor distribuído e utiliza o endereço (0x76);
4. As lâmpadas, aparelhos de ar, sensores de presença, de portas e janelas estão ligados aos pinos da GPIO conforme a Tabela 1.

| Item                                 | GPIO | Direção |
|--------------------------------------|:----:|:-------:|
| Lâmpada 01 (Cozinha)                 |  17  | Saída   |
| Lâmpada 02 (Sala)                    |  18  | Saída   |
| Lâmpada 03 (Quarto 01)               |  27  | Saída   |
| Lâmpada 04 (Quarto 02)               |  22  | Saída   |
| Ar-Condicionado 01 (Quarto 01)       |  13  | Saída   |
| Ar-Condicionado 02 (Quarto 02)       |  19  | Saída   |
| Sensor de Presença 01 (Sala)         |  25  | Entrada |
| Sensor de Presença 02 (Cozinha)      |  26  | Entrada |
| Sensor Abertura 01 (Porta Cozinha)   |  05  | Entrada |
| Sensor Abertura 02 (Janela Cozinha)  |  06  | Entrada |
| Sensor Abertura 03 (Porta Sala)      |  12  | Entrada |
| Sensor Abertura 04 (Janela Sala)     |  16  | Entrada |
| Sensor Abertura 05 (Janela Quarto 01)|  20  | Entrada |
| Sensor Abertura 06 (Janela Quarto 02)|  21  | Entrada |


## 4. Execução e compilação do Software

### Compilação do servidor distribuido

Vá para pasta src/distribuido/. Nesta pasta existe um Makefile que ajudará na compilação do projeto.

Para compilar use o comando abaixo:

```bash
make build
```

Para executar:

```bash
make run
```

O comando acima iniciará a conexão TCP com o IP definido no comando run. Para rodar em outro ambiente altere esse IP.
Uma alternativa é executar o binário após o build do software.

```bash
./project <ip>
```

### Execução do servidor Central

Mova para a pasta src/central/. O servidor central é implementado em python, para execução basta executar o comando abaixo.

```bash
python main.py
```

O servidor irá aguardar alguma conexão TCP feita pelo servidor distribuido e iniciará o software após isso.

## 5. Observações sobre a implementação do projeto

Alguns pontos foram deixados de lado nessa implementação:

* Execução do áudio
* Interface das entradas
* Envio customizado do input

Para diminuir a carência do último foi implementado um gerador de comandos válidos no servidor central que são enviados ao
servidor distribuído. Todas as mensagens enviadas são logadas no arquivo report.csv.

## 6. Referências

[Driver da Bosh para o sensor BME280](https://github.com/BoschSensortec/BME280_driver)

[Biblioteca BCM2835 - GPIO](http://www.airspayce.com/mikem/bcm2835/)
