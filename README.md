# 🏥 Care Plus – Edge Computing & IoT Health Tracker

Projeto desenvolvido para a **Sprint 2 do Challenge Care Plus – FIAP 2026**  
Disciplina: **Edge Computing & Computer Systems**

---

## 👥 Integrantes

| Nome | RM |
|---|---|
| Nicolas Forcione de Oliveira e Souza | RM566998 |
| Alexandre Constantino Furtado Junior | RM567188 |
| Enrico Dellatorre da Fonseca | RM566824 |
| Leonardo Batista de Souza | RM568558 |
| Matheus Freitas dos Santos | RM567337 |

---

## 📋 Descrição do Projeto

Dispositivo vestível (**Health Tracker**) baseado em **ESP32** que captura métricas de saúde simuladas (BPM, SpO2 e Temperatura) e as transmite via protocolo **MQTT** para a plataforma **FIWARE**, onde são armazenadas e disponibilizadas via API REST.

---

## 🏗️ Arquitetura da Solução

<img src="https://raw.githubusercontent.com/Nicolas-reach/EdgeComputing-Sprint2-CarePlus-/main/docs/arquitetura.png" width="800"/>
---

## 🔗 Links

- 🖥️ **Simulação Wokwi:** https://wokwi.com/projects/462510376052402177
- 🌐 **Orion Context Broker:** https://orion-production-769e.up.railway.app
- 🤖 **IoT Agent UL:** https://iotagent-ul-production.up.railway.app

---

## 🛠️ Tecnologias Utilizadas

| Tecnologia | Função |
|---|---|
| ESP32 | Microcontrolador do dispositivo |
| Wokwi | Simulador de hardware |
| MQTT (Ultralight 2.0) | Protocolo de comunicação IoT |
| Eclipse Mosquitto | MQTT Broker |
| FIWARE IoT Agent UL | Tradução de protocolo |
| FIWARE Orion Context Broker | Gerenciamento de contexto |
| MongoDB | Banco de dados |
| Railway | Plataforma de deploy (PaaS) |
| Docker / Docker Compose | Containerização local |

---

## 🚀 Como Executar Localmente

### Pré-requisitos
- [Docker](https://www.docker.com/) instalado
- [Docker Compose](https://docs.docker.com/compose/) instalado

### 1. Clone o repositório
```bash
git clone https://github.com/seu-usuario/EdgeComputing-Sprint2-CarePlus.git
cd EdgeComputing-Sprint2-CarePlus
```

### 2. Suba os serviços
```bash
docker compose up -d
```

Verifique se todos estão rodando:
```bash
docker compose ps
```

### 3. Registre o Service Group
```bash
curl -X POST http://localhost:4041/iot/services \
  -H "Content-Type: application/json" \
  -H "fiware-service: openiot" \
  -H "fiware-servicepath: /" \
  -d '{
    "services": [{
      "apikey": "4jggokgpepnvsb2uv4s40d59ov",
      "cbroker": "http://orion:1026",
      "entity_type": "Thing",
      "resource": ""
    }]
  }'
```

### 4. Registre o Device
```bash
curl -X POST http://localhost:4041/iot/devices \
  -H "Content-Type: application/json" \
  -H "fiware-service: openiot" \
  -H "fiware-servicepath: /" \
  -d '{
    "devices": [{
      "device_id": "sensor001",
      "entity_name": "urn:ngsi-ld:Thing:sensor001",
      "entity_type": "Thing",
      "transport": "MQTT",
      "attributes": [
        { "object_id": "bpm",  "name": "bpm",        "type": "Number" },
        { "object_id": "spo2", "name": "spo2",        "type": "Number" },
        { "object_id": "t",    "name": "temperature", "type": "Number" }
      ]
    }]
  }'
```

### 5. Teste enviando dados MQTT
Use o [MQTT Explorer](https://mqtt-explorer.com/) e publique:

Host:    localhost
Port:    1883
Topic:   /4jggokgpepnvsb2uv4s40d59ov/sensor001/attrs
Payload: bpm|72|spo2|98|t|36.5

### 6. Consulte os dados no Orion
```bash
curl -X GET http://localhost:1026/v2/entities \
  -H "fiware-service: openiot" \
  -H "fiware-servicepath: /"
```

---

## 📡 Simulação com Wokwi

1. Acesse: https://wokwi.com/projects/462510376052402177
2. No `sketch.ino`, atualize as credenciais MQTT:
```cpp
const char* mqtt_server = "SEU-HOST-MQTT";
const int   mqtt_port   = SUA-PORTA;
```
3. Pressione **Play** — o Serial Monitor exibirá as publicações

---

## 🌡️ Dados Monitorados

| Atributo | Descrição | Faixa |
|---|---|---|
| `bpm` | Frequência cardíaca | 50 – 120 bpm |
| `spo2` | Saturação de oxigênio | 90 – 100 % |
| `temperature` | Temperatura corporal | 36.0 – 39.0 °C |

---

## ⚙️ Variáveis de Ambiente (Produção – Railway)

```env
IOTA_CB_HOST=orion.railway.internal
IOTA_CB_PORT=1026
IOTA_CB_NGSI_VERSION=v2
IOTA_NORTH_HOST=0.0.0.0
IOTA_NORTH_PORT=4041
IOTA_REGISTRY_TYPE=mongodb
IOTA_MONGO_HOST=mongo.railway.internal
IOTA_MONGO_PORT=27017
IOTA_MONGO_DB=iotagentul
IOTA_MQTT_HOST=eclipse-mosquitto.railway.internal
IOTA_MQTT_PORT=1883
IOTA_AMQP_DISABLED=true
IOTA_LOG_LEVEL=DEBUG
IOTA_TIMESTAMP=true
IOTA_AUTOCAST=true
IOTA_DEFAULT_RESOURCE=
IOTA_PROVIDER_URL=https://iotagent-ul-production.up.railway.app
```


