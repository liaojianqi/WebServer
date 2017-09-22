package main

import (
	"io/ioutil"
	"errors"
	"time"
    "fmt"
    "net"
    "net/http"
    "strings"
    "crypto/sha1"
    "encoding/base64"
)

const SEC string = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
var clients []net.Conn

func getResponseStr(status int, kvs []string) (resp string) {
    if status == 101 {
        resp += "HTTP/1.1 101 Switching Protocols\r\n"
    } else if status == 400 {
        resp += "HTTP/1.1 400 Bad Request\r\n"
    }
    resp += "Date: " + time.Now().Format("2006-01-02 15:04:05") + "\r\n"
    resp += "Content-Type: text/html;charset=ISO-8859-1\r\n"
    resp += "Content-Length: 0\r\n"
    for _, kv := range kvs {
        resp += kv + "\r\n"
    }
    resp += "\r\n"
    return
}

func establishWs(conn net.Conn) error {
    buf := make([]byte, 1024)
    n, err := conn.Read(buf)
    if err != nil {
        return err
    }
    s := string(buf[:n])
    ss := strings.Split(s, "\r\n")
    isWs := false
    secKey := ""
    for _, v := range ss {
        kv := strings.Split(v, ": ")
        if len(kv) < 2 {
            continue
        }
        k, v := kv[0], kv[1]
        if k == "Upgrade" && v == "websocket" {
            isWs = true
        }
        if k == "Sec-WebSocket-Key" {
            secKey = v
        }
    }
    // response
    if isWs && secKey != "" {
        secKey += SEC
        buf := sha1.Sum([]byte(secKey))
        // base64
        s := base64.StdEncoding.EncodeToString(buf[:])
        fmt.Println(s)
        header := []string{
            "Upgrade: websocket",
            "Connection: Upgrade",
            "Sec-WebSocket-Accept: " + s,
        }
        resp := getResponseStr(101, header)
        conn.Write([]byte(resp))
        return nil
    } else {
        // 400
        resp := getResponseStr(400, []string{})
        conn.Write([]byte(resp))
        conn.Close()
        return errors.New("bad request")
    }
}

func simply(str []byte) (data []byte) {
    if len(str) <= 2 {
        return 
    }
    mask := make([]byte, 4)
    for i := 0; i < 4; i++ {
        mask[i] = str[i+2]
    }
    j := 0
    for i := 6; i < len(str); i++ {
        data = append(data, str[i] ^ mask[j])
        j = (j+1) % 4
    }
    return
}

func wrapper(str []byte) (data []byte) {
    data = append(data, 129)
    l := len(str)
    b := byte(l)
    data = append(data, b)
    for _, i := range str {
        data = append(data, i)
    }
    return
}

func process(conn net.Conn) {
    err := establishWs(conn)
    if err != nil {
        fmt.Println("establishWs error")
        return
    }
    clients = append(clients, conn)
    // rw
    for {
        buf := make([]byte, 1024)
        n, err := conn.Read(buf)
        if err != nil {
            panic(err)
        }
        // get data from net
        data := simply(buf[0:n])
        // send to others
        for _, v := range clients {
           if v == conn {
               continue
           }
           v.Write(wrapper(data))
        }
    }
}

func httpServer() {
    http.HandleFunc("/index", func(w http.ResponseWriter, r *http.Request) {
        buf, err := ioutil.ReadFile("../js/Demo01.html")
        if err != nil {
            panic(err)
        }
        fmt.Fprintf(w, string(buf))
    })
    http.ListenAndServe(":8888", nil)
}

func main() {
    go httpServer()
    ln, err := net.Listen("tcp", ":8080")
    if err != nil {
        fmt.Println(err)
        return
    }
    for {
        conn, err := ln.Accept()
        if err != nil {
            fmt.Println(err)
            continue
        }
        go process(conn)
    }
}
