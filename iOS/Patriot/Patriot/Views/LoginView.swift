//
//  LoginView.swift
//  Patriot
//
//  Created by Ron Lisle on 5/27/21.
//

import SwiftUI

struct LoginView: View {
    
    @Binding var needsLogin: Bool
    
    @State private var userName = ""
    @State private var password = ""
    
    @State private var hideSpinner = true
    
    var body: some View {
        ZStack {
            ProgressView().isHidden(hideSpinner)
            
            GeometryReader { metrics in
                VStack {
                    Spacer()
                    HStack {
                        Spacer()
                        VStack(alignment: .center, spacing: 20, content: {
                            Text("Login to your Particle.io account")
                                .padding(.horizontal, -4)
                                .padding(.vertical, 12)
                            TextField("User name", text: $userName)
                                .autocapitalization(.none)
                                .textFieldStyle(RoundedBorderTextFieldStyle())
                                .background(Color.gray)
                            TextField("Password", text: $password)
                                .autocapitalization(.none)
                                .textFieldStyle(RoundedBorderTextFieldStyle())
                                .background(Color.gray)
                            //TODO: submit button
                            Button(action: {
                                handleLogin()
                            }, label: {
                                Text("Login")
                                    .padding(.horizontal, 24)
                                    .padding(.vertical, 8)
                                    .background(Color.blue)
                                    .foregroundColor(Color.white)
                                    .cornerRadius(20)
                            })
                            .disabled(userName == "" || password == "")
                            .padding(.bottom, 10)
                        })
                        .padding(20)
                        .frame(width: metrics.size.width * 0.75, alignment: .center)
                        .background(Color.gray)
                        .cornerRadius(30)
                        .shadow(radius: /*@START_MENU_TOKEN@*/10/*@END_MENU_TOKEN@*/)
                        Spacer()
                    }
                    Spacer()
                }
            }
        }
    }
    
    func handleLogin() {
        hideSpinner = false
        PhotonManager.shared.login(user: userName, password: password) { (error) in
            hideSpinner = true
            if let error = error {
                print("Error logging in: \(error)")
                //TODO: display an error message, but stay on this screen
                return
            }
            needsLogin = false
        }
    }
}

struct PresentLoginView: View {
    @State var needsLogin = true
    var body: some View {
        LoginView(needsLogin: $needsLogin)
    }
}

struct LoginView_Previews: PreviewProvider {
    static var previews: some View {
        PresentLoginView()
    }
}
