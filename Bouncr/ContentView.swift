//
//  ContentView.swift
//  Bouncr
//
//  Created by Jack Frank on 7/25/22.
//

import SwiftUI
import UserNotifications
import CoreLocation

struct ContentView: View {
    @State private var showHome = false
    @State private var power = false
    @State private var sensitive = false
    @State private var alerted = false
    @State private var text1 = "LOCK YOUR CUP"
    @State private var text2 = "CLICK FOR HIGHER SENSITIVITY"
    @State private var color1 = Color.black
    @State private var color2 = Color.black
    let timer = Timer.publish(every: 1, on: .main, in: .common).autoconnect()
    var body: some View {
        ZStack {
            Image("yellow")
                .resizable()
                .aspectRatio(contentMode: .fill)
                .ignoresSafeArea(.all)
            if !showHome {
                VStack {
                    Image("logo")
                        .resizable()
                        .frame(width: 282, height: 100)
                    Text("Loading...")
                        .font(.custom("Avenir", fixedSize: 34).weight(.black))
                }
                .onAppear() {
                    UNUserNotificationCenter.current().requestAuthorization(options: [.alert, .badge, .sound]) { success, error in
                        if success {
                            print("All set!")
                        } else if let error = error {
                            print(error.localizedDescription)
                        }
                    }
                    DispatchQueue.main.asyncAfter(deadline: .now() + 1.5) {
                        showHome = true
                    }
                }
            } else {
                VStack {
                    Image("logo")
                        .resizable()
                        .frame(width: 282, height: 100)
                        .padding()
                        .position(x: UIScreen.main.bounds.width/2 ,y: UIScreen.main.bounds.width/2)
                        .onReceive(timer) { time in
                            if !alerted {
                                if let url = URL(string: "https://blynk.cloud/external/api/get?token=nNBPrdOAuy2iYulg2RK_9Dm9Ejsl4qt3&v2") {
                                    do {
                                        let contents = try String(contentsOf: url)
                                        //print(contents)
                                        if contents == "1" {
                                            alerted = true
                                            text1 = "TAMPERING DETECTED"
                                            color1 = .red
                                            let content = UNMutableNotificationContent()
                                            content.title = "ALERT"
                                            content.subtitle = "DRINK HAS BEEN TAMPERED WITH!!!"
                                            content.sound = UNNotificationSound.default
                                            let trigger = UNTimeIntervalNotificationTrigger(timeInterval: 5, repeats: false)
                                            let request = UNNotificationRequest(identifier: UUID().uuidString, content: content, trigger: trigger)
                                            UNUserNotificationCenter.current().add(request)
                                        }
                                    } catch {
                                    }
                                } else {
                                }
                            }
                        }
                    VStack {
                        Button(text1) {
                            if !power {
                                power = true
                                text1 = "CUP LOCKED"
                                color1 = .green
                                let url = URL(string: "https://blynk.cloud/external/api/update?token=nNBPrdOAuy2iYulg2RK_9Dm9Ejsl4qt3&v0=1")
                                let task = URLSession.shared.dataTask(with: url!, completionHandler: { (data, response, error) in
                                })
                                task.resume()
                            } else {
                                power = false
                                text1 = "LOCK YOUR CUP"
                                color1 = .black
                                let url = URL(string: "https://blynk.cloud/external/api/update?token=nNBPrdOAuy2iYulg2RK_9Dm9Ejsl4qt3&v0=0")
                                let task = URLSession.shared.dataTask(with: url!, completionHandler: { (data, response, error) in
                                })
                                task.resume()
                                DispatchQueue.main.asyncAfter(deadline: .now() + 1.5) {
                                    alerted = false
                                }
                            }
                        }
                        .padding()
                        .background(color1)
                        .foregroundColor(.white)
                        .clipShape(Capsule())
                        .font(.custom("Avenir", fixedSize: 26).weight(.black))
                        Button(text2) {
                            if !sensitive {
                                sensitive = true
                                text2 = "CLICK FOR LOWER SENSITIVITY"
                                color2 = .green
                                let url = URL(string: "https://blynk.cloud/external/api/update?token=nNBPrdOAuy2iYulg2RK_9Dm9Ejsl4qt3&v1=1")
                                let task = URLSession.shared.dataTask(with: url!, completionHandler: { (data, response, error) in
                                })
                                task.resume()
                            } else {
                                sensitive = false
                                text2 = "CLICK FOR HIGHER SENSITIVITY"
                                color2 = .black
                                let url = URL(string: "https://blynk.cloud/external/api/update?token=nNBPrdOAuy2iYulg2RK_9Dm9Ejsl4qt3&v1=0")
                                let task = URLSession.shared.dataTask(with: url!, completionHandler: { (data, response, error) in
                                })
                                task.resume()
                            }
                        }
                        .padding()
                        .background(color2)
                        .foregroundColor(.white)
                        .clipShape(Capsule())
                        .font(.custom("Avenir", fixedSize: 14).weight(.black))
                    }
                    .position(x: UIScreen.main.bounds.width/2 ,y: UIScreen.main.bounds.width/8)
                }
            }
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
