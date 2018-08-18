#pragma once

#include "NBody.hpp"

NBody::NBody()
{}

void NBody::setup(const int n)
{
	mNumParticles = n;
	initParticles(1);

	mParticlesBuffer.allocate(mParticles, GL_DYNAMIC_DRAW);
	mVbo.setVertexBuffer(mParticlesBuffer, 4, sizeof(Particle));
	mParticlesBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);

	mComputeShader.setupShaderFromFile(GL_COMPUTE_SHADER, "shaders/compute.comp");
	mComputeShader.linkProgram();
	mRenderShader.load("shaders/render");

	mGui.setup();
}

void NBody::update(const float t)
{
	mTime = t;

	mComputeShader.begin();
	mComputeShader.setUniform1i("uNumParticles", mNumParticles);
	mComputeShader.setUniform1f("uG", mG);
	mComputeShader.setUniform1f("uDt", mDt);
	mComputeShader.setUniform1f("uEPS2", mEPS2);
	mComputeShader.setUniform1f("uPower", mPower);
	mComputeShader.dispatchCompute(mNumParticles / WORK_GROUP_SIZE, 1, 1);
	// glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	mComputeShader.end();
}

void NBody::draw()
{
	mCam.begin();
	ofPushMatrix();
	ofTranslate(0, 0, 740);
	ofScale(20, 20);
	mRenderShader.begin();
	mVbo.draw(GL_POINTS, 0, mNumParticles);
	mRenderShader.end();
	ofPopMatrix();
	mCam.end();
}

void NBody::drawGui()
{
	const float offsetx = 10;
	const float offsety = 10;
	if(mIsGui)
	{
	    ofDisableDepthTest();
	    mGui.begin();
	    {
	        // Color Control
	        ImGui::SetNextWindowPos(ImVec2(offsetx, offsety));
	        ImGui::SetNextWindowSize(ImVec2(300, 130));
	        ImGui::Begin("N-Body Control");
	        {
				ImGui::SliderFloat("G", &mG, 0, 10);
				ImGui::SliderFloat("Dt", &mDt, 0, 2);
				ImGui::SliderFloat("EPS2", &mEPS2, 0, 1.0);
				ImGui::SliderFloat("Power", &mPower, 0, 5);
	        }
	        ImGui::End();
	    }
	    mGui.end();
	    ofEnableDepthTest();
	}
}

void NBody::initParticles(const int initMode)
{
	mParticles.resize(mNumParticles);

	std::mt19937 rng;
    rng.seed(std::random_device()());

	switch(initMode)
	{
	case 0:
	{
		// Initial particle positions
        std::normal_distribution<float> dist_distr(0.0f, 0.8f);
        std::normal_distribution<float> velocity_distr(0.0f, 0.5f);
        std::normal_distribution<float> mass_distr(0.5f, 0.1f);

        for (int i = 0; i < mNumParticles / 2; i++)
        {
            double mass = std::fabs(mass_distr(rng));//*2.0e30;

            ofVec3f angular_velocity(0, 0, -0.4);

            ofVec3f position_base(1.5, 0.1, 0.0);

            ofVec3f position(position_base.x + dist_distr(rng),
                             position_base.y + dist_distr(rng),
                             position_base.z + dist_distr(rng) * 0.1);

            ofVec3f velocity_base(0 + velocity_distr(rng) * 0.02,
                                  0 + velocity_distr(rng) * 0.02,
                                  0 + velocity_distr(rng) * 0.02);

            ofVec3f r = position - position_base;

            //ofVec3f velocity = ofVec3f::crossProduct(r, angular_velocity) + velocity_base;
            ofVec3f velocity =  r.cross(angular_velocity) + velocity_base;

			mParticles.at(i).pos.x = position.x;
			mParticles.at(i).pos.y = position.y;
			mParticles.at(i).pos.z = position.z;
			mParticles.at(i).pos.w = mass;//*2.0e30;
			mParticles.at(i).vel.x = velocity.x;
			mParticles.at(i).vel.y = velocity.y;
			mParticles.at(i).vel.z = velocity.z;
			mParticles.at(i).vel.w = 0.0;
        }


        for (int i = mNumParticles / 2; i < mNumParticles; i++)
        {
            double mass = std::fabs(mass_distr(rng));//*2.0e30;

            ofVec3f angular_velocity(0, 0, -0.4);

            ofVec3f position_base(-1.5, -0.1, 0.0);

            ofVec3f position(position_base.x + dist_distr(rng),
                             position_base.y + dist_distr(rng),
                             position_base.z + dist_distr(rng) * 0.1);

            ofVec3f velocity_base(0 + velocity_distr(rng) * 0.02,
                                  0 + velocity_distr(rng) * 0.02,
                                  0 + velocity_distr(rng) * 0.02);

            ofVec3f r = position - position_base;

            //ofVec3f velocity = ofVec3f::crossProduct(r, angular_velocity) + velocity_base;//.normalized()*r.length()*0.3;//*std::sqrt(particleComputeUbo.G*(mass + 3000)/r.length());// + velocity_base;
            ofVec3f velocity = r.cross(angular_velocity) + velocity_base;//.normalized()*r.length()*0.3;//*std::sqrt(particleComputeUbo.G*(mass + 3000)/r.length());// + velocity_base;


			mParticles.at(i).pos.x = position.x;
			mParticles.at(i).pos.y = position.y;
			mParticles.at(i).pos.z = position.z;
			mParticles.at(i).pos.w = mass;
			mParticles.at(i).vel.x = velocity.x;
			mParticles.at(i).vel.y = velocity.y;
			mParticles.at(i).vel.z = velocity.z;
			mParticles.at(i).vel.w = 0.0;
        }
        break;
	}
	case 1:
    {
        // Initial particle positions
        std::normal_distribution<float> dist_distr(0.0f, 0.7f);
        std::normal_distribution<float> velocity_distr(0.0f, 0.5f);
        std::normal_distribution<float> mass_distr(0.5f, 0.3f);

        for (int i = 0; i < mNumParticles / 2; i++)
        {
            double mass = std::fabs(mass_distr(rng));//*2.0e30;

            ofVec3f angular_velocity(0, 0, -0.4);

            ofVec3f position_base(1.5, 0.1, 0.0);

            ofVec3f position(position_base.x + dist_distr(rng),
                             position_base.y + dist_distr(rng),
                             position_base.z + dist_distr(rng) * 0.1);

            ofVec3f velocity_base(0 + velocity_distr(rng) * 0.02,
                                  0 + velocity_distr(rng) * 0.02,
                                  0 + velocity_distr(rng) * 0.02);

            ofVec3f r = position - position_base;

            //ofVec3f velocity = sqrt(ubo_nbody_compute.gravity_constant * (9500) / r.length()) * ofVec3f::crossProduct(r, angular_velocity).normalized();// + velocity_base;
            ofVec3f velocity = sqrt(mG * (9500) / r.length()) * r.cross(angular_velocity).normalized();// + velocity_base;

			mParticles.at(i).pos.x = position.x;
			mParticles.at(i).pos.y = position.y;
			mParticles.at(i).pos.z = position.z;
			mParticles.at(i).pos.w = mass;//*2.0e30;
			mParticles.at(i).vel.x = velocity.x;
			mParticles.at(i).vel.y = velocity.y;
			mParticles.at(i).vel.z = velocity.z;
			mParticles.at(i).vel.w = 0.0;

            if (i == 0)
            {
				mParticles.at(i).pos.x = position_base.x;
				mParticles.at(i).pos.y = position_base.y;
				mParticles.at(i).pos.z = position_base.z;
				mParticles.at(i).pos.w = 9500;//*2.0e30;
				mParticles.at(i).vel.x = 0.0;
				mParticles.at(i).vel.y = 0.0;
				mParticles.at(i).vel.z = 0.0;
				mParticles.at(i).vel.w = 0.0;
            }
        }


        for (int i = mNumParticles / 2; i < mNumParticles; i++)
        {
            double mass = std::fabs(mass_distr(rng));//*2.0e30;

            ofVec3f angular_velocity(0, 0, -0.4);

            ofVec3f position_base(-1.5, -0.1, 0.0);

            ofVec3f position(position_base.x + dist_distr(rng),
                               position_base.y + dist_distr(rng),
                               position_base.z + dist_distr(rng) * 0.1);

            ofVec3f velocity_base(0 + velocity_distr(rng) * 0.02,
                                    0 + velocity_distr(rng) * 0.02,
                                    0 + velocity_distr(rng) * 0.02);

            ofVec3f r = position - position_base;

            //ofVec3f velocity = sqrt(ubo_nbody_compute.gravity_constant * (10000) / r.length()) * ofVec3f::crossProduct(r, angular_velocity).normalized();// + velocity_base;
            ofVec3f velocity = sqrt(mG * (10000) / r.length()) * r.cross(angular_velocity).normalized();// + velocity_base;

			mParticles.at(i).pos.x = position.x;
			mParticles.at(i).pos.y = position.y;
			mParticles.at(i).pos.z = position.z;
			mParticles.at(i).pos.w = mass;//*2.0e30;
			mParticles.at(i).vel.x = velocity.x;
			mParticles.at(i).vel.y = velocity.y;
			mParticles.at(i).vel.z = velocity.z;
			mParticles.at(i).vel.w = 0.0;

            if (i == mNumParticles / 2)
            {
				mParticles.at(i).pos.x = position_base.x;
				mParticles.at(i).pos.y = position_base.y;
				mParticles.at(i).pos.z = position_base.z;
				mParticles.at(i).pos.w = 10000;
				mParticles.at(i).vel.x = 0.0;
				mParticles.at(i).vel.y = 0.0;
				mParticles.at(i).vel.z = 0.0;
				mParticles.at(i).vel.w = 0.0;
            }
        }
        break;
    }
    case 2:
    {
        // Initial particle positions
        std::normal_distribution<float> dist_distr(0.0f, 0.5f);
        std::normal_distribution<float> velocity_distr(0.0f, 0.5f);
        std::normal_distribution<float> mass_distr(0.5f, 0.1f);

        for (int i = 0; i < mNumParticles; i++)
        {
            double mass = std::fabs(mass_distr(rng));//*2.0e30;

            ofVec3f angular_velocity(0, 0, 0.4);

            ofVec3f position_base(0, 0, 0);

            ofVec3f position(position_base.x + dist_distr(rng),
                               position_base.y + dist_distr(rng),
                               position_base.z + dist_distr(rng));

            ofVec3f velocity_base(0 + velocity_distr(rng) * 0.02,
                                    0 + velocity_distr(rng) * 0.02,
                                    0 + velocity_distr(rng) * 0.02);

            ofVec3f r = position - position_base;

            //ofVec3f velocity = sqrt(ubo_nbody_compute.gravity_constant * (10000) / r.length()) * ofVec3f::crossProduct(r, angular_velocity).normalized();// + velocity_base;
            ofVec3f velocity = sqrt(mG * (10000) / r.length()) * r.cross(angular_velocity).normalized();// + velocity_base;

			mParticles.at(i).pos.x = position.x;
			mParticles.at(i).pos.y = position.y;
			mParticles.at(i).pos.z = position.z;
			mParticles.at(i).pos.w = mass;//*2.0e30;
			mParticles.at(i).vel.x = velocity.x;
			mParticles.at(i).vel.y = velocity.y;
			mParticles.at(i).vel.z = velocity.z;
			mParticles.at(i).vel.w = 0.0;

            if (i == 0)
            {
				mParticles.at(i).pos.x = position_base.x;
				mParticles.at(i).pos.y = position_base.y;
				mParticles.at(i).pos.z = position_base.z;
				mParticles.at(i).pos.w = 10000;
				mParticles.at(i).vel.x = 0.0;
				mParticles.at(i).vel.y = 0.0;
				mParticles.at(i).vel.z = 0.0;
				mParticles.at(i).vel.w = 0.0;
            }
        }
        break;
    }
    case 3:
    {
        // Initial particle positions
        std::normal_distribution<float> dist_distr(0.0f, 0.001f);
        std::normal_distribution<float> mass_distr(0.5f, 0.1f);

        int side = static_cast<int>(std::ceil(std::pow(static_cast<double>(mNumParticles), 1.0 / 3.0)));

        if (side < 1)
        {
            side = 1;
        }

        for (int i = 0; i < side; i++)
        {
            for (int j = 0; j < side; j++)
            {
                for (int k = 0; k < side; k++)
                {
                    int index = i * side * side + j * side + k;

                    if (index < mNumParticles)
                    {
                        double mass = std::fabs(mass_distr(rng));

                        ofVec3f position_base((static_cast<double>(i) - static_cast<double>(side - 1) * 0.5) / static_cast<double>(side - 1) * 2,
                                                (static_cast<double>(j) - static_cast<double>(side - 1) * 0.5) / static_cast<double>(side - 1) * 2,
                                                (static_cast<double>(k) - static_cast<double>(side - 1) * 0.5) / static_cast<double>(side - 1) * 2);

                        ofVec3f position(position_base.x + dist_distr(rng),
                                           position_base.y + dist_distr(rng),
                                           position_base.z + dist_distr(rng));

						mParticles.at(i).pos.x = position.x;
						mParticles.at(i).pos.y = position.y;
						mParticles.at(i).pos.z = position.z;
						mParticles.at(i).pos.w = mass;//*2.0e30;
						mParticles.at(i).vel.x = 0.0;
						mParticles.at(i).vel.y = 0.0;
						mParticles.at(i).vel.z = 0.0;
						mParticles.at(i).vel.w = 0.0;
                    }
                }
            }
        }
        break;
    }
    case 4:
    {
        // Initial particle positions
        std::normal_distribution<float> dist_distr(0.0f, 0.001f);
        std::normal_distribution<float> mass_distr(0.5f, 0.1f);

        int side = static_cast<int>(std::ceil(std::pow(static_cast<double>(mNumParticles), 1.0 / 3.0)));

        if (side < 1)
        {
            side = 1;
        }

        for (int i = 0; i < side; i++)
        {
            for (int j = 0; j < side; j++)
            {
                for (int k = 0; k < side; k++)
                {
                    int index = i * side * side + j * side + k;

                    if (index < mNumParticles)
                    {
                        double mass = std::fabs(mass_distr(rng));

                        ofVec3f position_base((static_cast<double>(i) - static_cast<double>(side - 1) * 0.5) / static_cast<double>(side - 1) * 2,
                                                (static_cast<double>(j) - static_cast<double>(side - 1) * 0.5) / static_cast<double>(side - 1) * 2,
                                                (static_cast<double>(k) - static_cast<double>(side - 1) * 0.5) / static_cast<double>(side - 1) * 2);

                        ofVec3f position(position_base.x + dist_distr(rng),
                                           position_base.y + dist_distr(rng),
                                           position_base.z + dist_distr(rng));

						mParticles.at(i).pos.x = position.x;
						mParticles.at(i).pos.y = position.y;
						mParticles.at(i).pos.z = position.z;
						mParticles.at(i).pos.w = mass;
						mParticles.at(i).vel.x = 5.0 * std::sin(static_cast<double>(j) / static_cast<double>(side - 1) * 100);
						mParticles.at(i).vel.y = 0.0;
						mParticles.at(i).vel.z = 0.0;
						mParticles.at(i).vel.w = 0.0;
                    }
                }
            }
        }
        break;
    }
	case 5:
	{
		// Initial particle positions
		std::normal_distribution<float> dist_distr(0.0f, 1.0f);
    	std::normal_distribution<float> mass_distr(0.5f, 0.1f);

    	int blob_count       = 20;
    	int blob_point_count = mNumParticles / blob_count;

		for (int i = 0; i < blob_count; i++)
		{
			ofVec3f position_base(dist_distr(rng), dist_distr(rng), dist_distr(rng));

			for (int j = 0; j < blob_point_count; j++)
			{
				int index = i * blob_point_count + j;

				if (index < mNumParticles)
				{
					ofVec3f position(position_base.x + dist_distr(rng) * 0.2,
						position_base.y + dist_distr(rng) * 0.2,
						position_base.z + dist_distr(rng) * 0.2);

					double mass = std::fabs(mass_distr(rng));

					mParticles.at(index).pos.x = position.x;
					mParticles.at(index).pos.y = position.y;
					mParticles.at(index).pos.z = position.z;
					mParticles.at(index).pos.w = mass;
					mParticles.at(index).vel.x = 0;
					mParticles.at(index).vel.y = 0;
					mParticles.at(index).vel.z = 0;
					mParticles.at(index).vel.w = 0;
				}
			}
		}
		break;
	}
	default:
		break;
	}

}