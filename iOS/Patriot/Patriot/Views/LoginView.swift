//
//  LoginView.swift
//  Patriot
//
//  Created by Ron Lisle on 5/27/21.
//

import SwiftUI

struct LoginView: View {
    
    @Binding var needsLogin: Bool
    
    @State var userName = ""
    @State var password = ""
    
    var body: some View {
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
                            .textFieldStyle(RoundedBorderTextFieldStyle())
                            .background(Color.gray)
                        TextField("Password", text: $password)
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
    
    func handleLogin() {
        needsLogin = false
    }
}

struct CardView: View {
    var body: some View {
        GeometryReader { gp in
            VStack {
                VStack {
                    Text("Blue")
                }
                .frame(width: gp.size.width, height: gp.size.height * 0.7)
                .background(Color.gray)
            }
        }
        .frame(height: 280).frame(maxWidth: .infinity)
        .cornerRadius(24).padding(.horizontal, 30)
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
